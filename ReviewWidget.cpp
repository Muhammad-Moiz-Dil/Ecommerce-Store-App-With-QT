#include "../include/ReviewWidget.h"
#include <fstream>
#include <sstream>
#include <QDir>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSpacerItem>
#include <ctime>
#include <iostream>
#include "../../include/User.h"
#include "../../include/Order.h"

ReviewWidget::ReviewWidget(int productId, int userId, QWidget *parent)
    : QWidget(parent), productId(productId), userId(userId)
{
    setupUI();
    loadReviews();
}

ReviewWidget::~ReviewWidget()
{
}

void ReviewWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 1. Existing Reviews Section
    QGroupBox *reviewsGroup = new QGroupBox("Product Reviews", this);
    QVBoxLayout *reviewsLayout = new QVBoxLayout(reviewsGroup);
    
    reviewsListWidget = new QListWidget(this);
    reviewsListWidget->setAlternatingRowColors(true);
    reviewsLayout->addWidget(reviewsListWidget);
    
    // 2. Add Review Section
    QGroupBox *addReviewGroup = new QGroupBox("Add Your Review", this);
    QVBoxLayout *addReviewLayout = new QVBoxLayout(addReviewGroup);
    
    // Rating selection
    QHBoxLayout *ratingLayout = new QHBoxLayout();
    QLabel *ratingLabel = new QLabel("Rating:", this);
    ratingComboBox = new QComboBox(this);
    for (int i = 1; i <= 5; i++) {
        ratingComboBox->addItem(QString("%1 Star%2").arg(i).arg(i > 1 ? "s" : ""));
    }
    ratingLayout->addWidget(ratingLabel);
    ratingLayout->addWidget(ratingComboBox);
    ratingLayout->addStretch();
    
    // Comment text area
    QLabel *commentLabel = new QLabel("Comment:", this);
    commentTextEdit = new QTextEdit(this);
    commentTextEdit->setPlaceholderText("Write your review here...");
    commentTextEdit->setMaximumHeight(100);
    
    // Submit button
    submitButton = new QPushButton("Submit Review", this);
    submitButton->setMaximumWidth(150);
    
    // Add widgets to layout
    addReviewLayout->addLayout(ratingLayout);
    addReviewLayout->addWidget(commentLabel);
    addReviewLayout->addWidget(commentTextEdit);
    addReviewLayout->addWidget(submitButton, 0, Qt::AlignRight);
    
    // Connect button to slot
    connect(submitButton, &QPushButton::clicked, this, &ReviewWidget::onSubmitReview);
    
    // Add all sections to main layout
    mainLayout->addWidget(reviewsGroup);
    mainLayout->addWidget(addReviewGroup);
    
    // Check if user is logged in and has purchased this product
    if (userId <= 0) {
        // User not logged in
        addReviewGroup->setVisible(false);
        mainLayout->addWidget(new QLabel("Please log in to write a review.", this));
    } else {
        // User logged in, now check if they've purchased the product
        updateReviewEligibility();
    }
}

void ReviewWidget::loadReviews()
{
    reviewsListWidget->clear();
    
    // Instead of trying to use Review::getReviewsForProduct which returns void,
    // read the reviews directly from the file
    ifstream reviewFile("data/reviews/reviews.txt");
    if (!reviewFile) {
        reviewsListWidget->addItem("No reviews yet for this product.");
        return;
    }
    
    int reviewCount = 0;
    string line;
    
    while (getline(reviewFile, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string segment;
        int currentProdID, reviewUserId, rating;
        string commentStr;
        
        getline(ss, segment, ','); 
        try { currentProdID = stoi(segment); } catch(...) { continue; }
        
        if (currentProdID != productId) continue;
        
        getline(ss, segment, ','); 
        try { reviewUserId = stoi(segment); } catch(...) { continue; }
        
        getline(ss, segment, ','); 
        try { rating = stoi(segment); } catch(...) { continue; }
        
        getline(ss, commentStr); // Rest is comment
        
        reviewCount++;
        
        // Get user's email/name for display
        string userEmail;
        bool foundUser = User::getUserEmailById(reviewUserId, userEmail);
        QString userDisplayName = foundUser ? QString::fromStdString(userEmail) : QString("User #%1").arg(reviewUserId);
        
        // Format the review item
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, reviewUserId); // Store user ID for potential later use
        
        // Create formatted review text
        QString reviewText = QString("<b>%1</b> - <span style='color: goldenrod;'>%2★</span><br>%3")
            .arg(userDisplayName)
            .arg(rating)
            .arg(QString::fromStdString(commentStr));
        
        // Use a QLabel to render HTML
        QLabel *reviewLabel = new QLabel(reviewText);
        reviewLabel->setTextFormat(Qt::RichText); // Ensure HTML is rendered
        reviewLabel->setWordWrap(true); // Optional: ensure long reviews wrap

        // Add the item to the list first, then set its widget
        reviewsListWidget->addItem(item);
        reviewsListWidget->setItemWidget(item, reviewLabel);
        item->setSizeHint(reviewLabel->sizeHint()); // Ensure item size is updated
    }
    
    reviewFile.close();
    
    // Show message if no reviews
    if (reviewCount == 0) {
        reviewsListWidget->addItem("No reviews yet for this product.");
    }
}

void ReviewWidget::updateReviewEligibility()
{
    // Check if user has purchased this product
    bool hasPurchased = false;
    
    // Get orders for this user
    int count = 0;
    int* productIds = nullptr;
    
    // Find all orders for this user
    ifstream orderFile("data/orders/orders.txt");
    if (orderFile) {
        string line;
        while (getline(orderFile, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string segment;
            
            getline(ss, segment, ','); // Order ID
            getline(ss, segment, ','); // User ID
            
            int orderUserId = 0;
            try {
                orderUserId = stoi(segment);
            } catch (...) {
                continue;
            }
            
            if (orderUserId == userId) {
                // Found an order for this user, get product IDs
                int orderID = 0;
                try {
                    ss.clear();
                    ss.str(line);
                    getline(ss, segment, ','); // Order ID
                    orderID = stoi(segment);
                } catch (...) {
                    continue;
                }
                
                int productCount = 0;
                int* orderProductIds = Order::getProductIDsForOrder(orderID, productCount);
                
                if (orderProductIds) {
                    // Check if this product is in the order
                    for (int i = 0; i < productCount; i++) {
                        if (orderProductIds[i] == productId) {
                            hasPurchased = true;
                            delete[] orderProductIds;
                            orderFile.close();
                            goto eligibilityCheckEnd; // Break out of nested loops
                        }
                    }
                    delete[] orderProductIds;
                }
            }
        }
        orderFile.close();
    }
    
eligibilityCheckEnd:
    // Find the "Add Your Review" group box
    QList<QGroupBox*> groupBoxes = findChildren<QGroupBox*>();
    QGroupBox* addReviewGroup = nullptr;
    for (QGroupBox* box : groupBoxes) {
        if (box->title() == "Add Your Review") {
            addReviewGroup = box;
            break;
        }
    }
    
    if (addReviewGroup) {
        // Enable/disable based on purchase status
        addReviewGroup->setEnabled(hasPurchased);
        
        if (!hasPurchased) {
            // Add a note explaining why user can't review
            QLabel* noteLabel = new QLabel("You can only review products you have purchased.", this);
            noteLabel->setStyleSheet("color: red;");
            noteLabel->setAlignment(Qt::AlignCenter);
            
            // Add to main layout without modifying the review group
            QBoxLayout* mainLayout = qobject_cast<QBoxLayout*>(layout());
            if (mainLayout) {
                // Check if we already added this note
                bool noteExists = false;
                for (int i = 0; i < mainLayout->count(); ++i) {
                    QWidget* widget = mainLayout->itemAt(i)->widget();
                    QLabel* label = qobject_cast<QLabel*>(widget);
                    if (label && label->text().contains("can only review")) {
                        noteExists = true;
                        break;
                    }
                }
                
                if (!noteExists) {
                    mainLayout->addWidget(noteLabel);
                }
            }
        }
    }
}

void ReviewWidget::onSubmitReview()
{
    if (userId <= 0) {
        QMessageBox::information(this, "Login Required", "Please log in to submit a review.");
        return;
    }
    
    // Get the rating (1-5)
    int rating = ratingComboBox->currentIndex() + 1; // Index 0 = 1 star, etc.
    
    // Get the comment text
    QString commentQStr = commentTextEdit->toPlainText().trimmed();
    if (commentQStr.isEmpty()) {
        QMessageBox::warning(this, "Missing Comment", "Please provide a comment with your review.");
        return;
    }
    
    // Convert to C string for Review class
    std::string commentStr = commentQStr.toStdString();
    const char* commentCStr = commentStr.c_str();
    
    // Submit the review using Review's static method
    bool success = Review::addReview(productId, userId, rating, commentCStr);
    
    if (success) {
        QMessageBox::information(this, "Review Submitted", "Thank you for your review!");
        commentTextEdit->clear();
        loadReviews(); // Refresh the reviews list
        emit reviewAdded(productId);
    } else {
        QMessageBox::warning(this, "Review Failed", 
            "Unable to submit your review. You may have already reviewed this product.");
    }
} 