#include "../include/WishlistWidget.h"
#include <QFileInfo>
#include <QDir>

WishlistWidget::WishlistWidget(int userId, QWidget *parent)
    : QWidget(parent), userId(userId)
{
    // Ensure the wishlist directory exists
    QDir().mkpath("data/wishlist");
    
    setupUI();
    
    // Connect signals and slots
    connect(removeButton, &QPushButton::clicked, this, &WishlistWidget::handleRemoveFromWishlist);
    connect(moveToCartButton, &QPushButton::clicked, this, &WishlistWidget::handleMoveToCart);
    connect(wishlistTableWidget, &QTableWidget::cellDoubleClicked, this, &WishlistWidget::handleProductDoubleClicked);
    
    // Load initial wishlist items
    loadWishlistItems();
}

WishlistWidget::~WishlistWidget()
{
    // Qt handles cleanup of child widgets
}

void WishlistWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title label
    QLabel *titleLabel = new QLabel("<h2>My Wishlist</h2>", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Table for wishlist items
    wishlistTableWidget = new QTableWidget(this);
    wishlistTableWidget->setColumnCount(3);
    wishlistTableWidget->setHorizontalHeaderLabels({"Product Name", "Category", "Price"});
    wishlistTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    wishlistTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    wishlistTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    wishlistTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only
    wishlistTableWidget->setAlternatingRowColors(true);
    mainLayout->addWidget(wishlistTableWidget);
    
    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    removeButton = new QPushButton("Remove from Wishlist", this);
    moveToCartButton = new QPushButton("Move to Cart", this);
    
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(moveToCartButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Set the layout
    setLayout(mainLayout);
}

void WishlistWidget::loadWishlistItems()
{
    // Clear existing table
    wishlistTableWidget->setRowCount(0);
    
    // Create wishlist object using user ID
    Wishlist wishlist(userId);
    
    // Create the file path for the wishlist
    std::string wishlistFilePath = "data/wishlist/wishlist_" + std::to_string(userId) + ".txt";
    QFileInfo wishlistFile(QString::fromStdString(wishlistFilePath));
    
    // Check if wishlist file exists
    if (!wishlistFile.exists() || wishlistFile.size() == 0) {
        // Show empty message in table
        wishlistTableWidget->setRowCount(1);
        QTableWidgetItem *emptyItem = new QTableWidgetItem("Your wishlist is empty");
        emptyItem->setTextAlignment(Qt::AlignCenter);
        wishlistTableWidget->setSpan(0, 0, 1, 3);
        wishlistTableWidget->setItem(0, 0, emptyItem);
        
        // Disable buttons
        removeButton->setEnabled(false);
        moveToCartButton->setEnabled(false);
        
        return;
    }
    
    // Read wishlist data from file
    std::ifstream wishlistFileStream(wishlistFilePath);
    if (!wishlistFileStream.is_open()) {
        QMessageBox::warning(this, "Error", "Failed to open wishlist file.");
        return;
    }
    
    std::string line;
    int row = 0;
    
    while (std::getline(wishlistFileStream, line)) {
        if (line.empty()) continue;
        
        // Parse product ID
        int productId = 0;
        try {
            productId = std::stoi(line);
        } catch (...) {
            continue; // Skip invalid entries
        }
        
        if (productId <= 0) {
            continue; // Skip invalid entries
        }
        
        // Get product details using the Product class
        Product* product = Product::getProductByID(productId);
        if (product) {
            // Add a new row
            wishlistTableWidget->insertRow(row);
            
            // Create name item with hidden product ID
            QTableWidgetItem *nameItem = new QTableWidgetItem(product->getName() ? QString(product->getName()) : "Unknown Product");
            nameItem->setData(Qt::UserRole, productId); // Store product ID for later use
            
            // Create category and price items
            QTableWidgetItem *categoryItem = new QTableWidgetItem(product->getCategory() ? QString(product->getCategory()) : "");
            QTableWidgetItem *priceItem = new QTableWidgetItem(QString::asprintf("$%.2f", product->getPrice()));
            
            // Add items to the row
            wishlistTableWidget->setItem(row, 0, nameItem);
            wishlistTableWidget->setItem(row, 1, categoryItem);
            wishlistTableWidget->setItem(row, 2, priceItem);
            
            // Clean up
            delete product;
            row++;
        }
    }
    
    wishlistFileStream.close();
    
    // Enable/disable buttons based on content
    bool hasItems = (wishlistTableWidget->rowCount() > 0);
    removeButton->setEnabled(hasItems);
    moveToCartButton->setEnabled(hasItems);
    
    // Auto-resize columns to content
    wishlistTableWidget->resizeColumnsToContents();
}

void WishlistWidget::refreshWishlist()
{
    loadWishlistItems();
}

void WishlistWidget::handleRemoveFromWishlist()
{
    int currentRow = wishlistTableWidget->currentRow();
    if (currentRow < 0 || currentRow >= wishlistTableWidget->rowCount()) {
        QMessageBox::warning(this, "Remove Item", "Please select an item to remove.");
        return;
    }
    
    // Get the product ID from the selected row
    QTableWidgetItem* item = wishlistTableWidget->item(currentRow, 0);
    int productId = item->data(Qt::UserRole).toInt();
    
    // Use the backend Wishlist class to remove the item
    Wishlist wishlist(userId);
    if (wishlist.removeFromWishlist(productId)) {
        QMessageBox::information(this, "Remove Item", "Item removed from wishlist.");
        
        // Refresh the display
        loadWishlistItems();
    } else {
        QMessageBox::warning(this, "Error", "Failed to remove item from wishlist.");
    }
}

void WishlistWidget::handleMoveToCart()
{
    int currentRow = wishlistTableWidget->currentRow();
    if (currentRow < 0 || currentRow >= wishlistTableWidget->rowCount()) {
        QMessageBox::warning(this, "Move to Cart", "Please select an item to move to cart.");
        return;
    }
    
    // Get the product ID from the selected row
    QTableWidgetItem* item = wishlistTableWidget->item(currentRow, 0);
    int productId = item->data(Qt::UserRole).toInt();
    
    // Add to cart
    ShoppingCart cart(userId);
    if (cart.addToCart(productId, 1)) {
        // If successful, remove from wishlist
        Wishlist wishlist(userId);
        wishlist.removeFromWishlist(productId);
        
        QMessageBox::information(this, "Move to Cart", "Item moved from wishlist to cart.");
        
        // Refresh the display
        loadWishlistItems();
    } else {
        QMessageBox::warning(this, "Error", "Failed to add item to cart. It may be out of stock.");
    }
}

void WishlistWidget::handleProductDoubleClicked(int row, int column)
{
    if (row < 0 || row >= wishlistTableWidget->rowCount()) {
        return;
    }
    
    // Get the product ID from the selected row
    QTableWidgetItem* item = wishlistTableWidget->item(row, 0);
    int productId = item->data(Qt::UserRole).toInt();
    
    // Emit signal to show product details
    emit productSelected(productId);
} 