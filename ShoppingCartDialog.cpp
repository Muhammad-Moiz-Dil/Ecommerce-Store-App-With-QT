#include "../include/ShoppingCartDialog.h"
#include "../include/OrderSummaryDialog.h"
#include <QGridLayout>
#include <QStandardItem>
#include <QString>
#include <QFileInfo>
#include <QDir>

ShoppingCartDialog::ShoppingCartDialog(int userId, QWidget *parent)
    : QDialog(parent), userId(userId)
{
    setWindowTitle("Shopping Cart");
    
    // Create needed directories
    QDir().mkpath("data/cart");
    
    setupUI();
    
    // Connect signals
    connect(removeButton, &QPushButton::clicked, this, &ShoppingCartDialog::handleRemoveItem);
    connect(updateButton, &QPushButton::clicked, this, &ShoppingCartDialog::handleUpdateQuantity);
    connect(checkoutButton, &QPushButton::clicked, this, &ShoppingCartDialog::handleProceedToCheckout);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    // Load and display initial cart items
    loadCartItems();
    updateCartDisplay();
}

ShoppingCartDialog::~ShoppingCartDialog()
{
    // Qt handles deleting child widgets
}

void ShoppingCartDialog::setupUI()
{
    setMinimumWidth(600);
    setMinimumHeight(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Cart Table
    cartTableWidget = new QTableWidget(this);
    cartTableWidget->setColumnCount(4);
    cartTableWidget->setHorizontalHeaderLabels({"Product Name", "Quantity", "Price/Item", "Subtotal"});
    cartTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    cartTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only table
    mainLayout->addWidget(cartTableWidget);

    // Quantity update widgets
    QHBoxLayout *quantityLayout = new QHBoxLayout();
    QLabel *quantityLabel = new QLabel("Update Quantity:", this);
    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setMaximum(99);  // Reasonable limit
    quantitySpinBox->setValue(1);
    updateButton = new QPushButton("Update", this);
    
    quantityLayout->addWidget(quantityLabel);
    quantityLayout->addWidget(quantitySpinBox);
    quantityLayout->addWidget(updateButton);
    quantityLayout->addStretch();
    mainLayout->addLayout(quantityLayout);
    
    // Buttons for table actions
    QHBoxLayout *tableActionLayout = new QHBoxLayout();
    removeButton = new QPushButton("Remove Selected Item", this);
    tableActionLayout->addWidget(removeButton);
    tableActionLayout->addStretch();
    mainLayout->addLayout(tableActionLayout);

    // Total Label
    totalLabel = new QLabel("Total: $0.00", this);
    QFont totalFont = totalLabel->font();
    totalFont.setBold(true);
    totalFont.setPointSize(totalFont.pointSize() + 2);
    totalLabel->setFont(totalFont);
    totalLabel->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(totalLabel);

    // Dialog Action Buttons
    QHBoxLayout *dialogActionLayout = new QHBoxLayout();
    closeButton = new QPushButton("Close", this);
    checkoutButton = new QPushButton("Proceed to Checkout", this);
    dialogActionLayout->addStretch();
    dialogActionLayout->addWidget(closeButton);
    dialogActionLayout->addWidget(checkoutButton);
    mainLayout->addLayout(dialogActionLayout);

    setLayout(mainLayout);
}

void ShoppingCartDialog::loadCartItems()
{
    // Clear current items
    currentCartItems.clear();
    
    // Use the backend ShoppingCart class to get cart data
    ShoppingCart cart(userId);
    
    // Create the file path for the cart
    std::string cartFilePath = "data/cart/cart_" + std::to_string(userId) + ".txt";
    QFileInfo cartFile(QString::fromStdString(cartFilePath));
    
    // Check if cart file exists
    if (!cartFile.exists() || cartFile.size() == 0) {
        return; // Cart is empty
    }
    
    // Read cart data from file
    std::ifstream cartFileStream(cartFilePath);
    if (!cartFileStream.is_open()) {
        QMessageBox::warning(this, "Error", "Failed to open cart file.");
        return;
    }
    
    std::string line;
    while (std::getline(cartFileStream, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string segment;
        int productId = 0;
        int quantity = 0;
        
        // Parse product ID
        if (std::getline(ss, segment, ',')) {
            try {
                productId = std::stoi(segment);
            } catch (...) {
                continue; // Skip invalid entries
            }
        }
        
        // Parse quantity
        if (std::getline(ss, segment, ',')) {
            try {
                quantity = std::stoi(segment);
            } catch (...) {
                continue; // Skip invalid entries
            }
        }
        
        if (productId <= 0 || quantity <= 0) {
            continue; // Skip invalid entries
        }
        
        // Get product details using the Product class
        Product* product = Product::getProductByID(productId);
        if (product) {
            // Create a cart item with product details
            PlaceholderCartItem item;
            item.setProductId(productId);
            item.setName(product->getName() ? QString(product->getName()) : "Unknown Product");
            item.setQuantity(quantity);
            item.setPricePerItem(product->getPrice());
            
            // Add to our cart items vector
            currentCartItems.push_back(item);
            
            // Clean up
            delete product;
        }
    }
    
    cartFileStream.close();
}

void ShoppingCartDialog::updateCartDisplay()
{
    cartTableWidget->setRowCount(0); // Clear table
    cartTableWidget->setRowCount(currentCartItems.size());

    for (size_t i = 0; i < currentCartItems.size(); ++i) {
        const auto& item = currentCartItems[i];
        
        // Store the product ID in the first column as hidden data
        QTableWidgetItem* nameItem = new QTableWidgetItem(item.getName());
        nameItem->setData(Qt::UserRole, item.getProductId()); // Store product ID for removal/updates
        
        cartTableWidget->setItem(i, 0, nameItem);
        cartTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(item.getQuantity())));
        cartTableWidget->setItem(i, 2, new QTableWidgetItem(QString::asprintf("$%.2f", item.getPricePerItem())));
        cartTableWidget->setItem(i, 3, new QTableWidgetItem(QString::asprintf("$%.2f", item.getQuantity() * item.getPricePerItem())));
    }
    
    // Enable/disable checkout based on cart contents
    checkoutButton->setEnabled(!currentCartItems.empty());
    
    // Update total
    totalLabel->setText(QString("Total: $%1").arg(calculateTotal(), 0, 'f', 2));
}

double ShoppingCartDialog::calculateTotal()
{
    double total = 0.0;
    for (const auto& item : currentCartItems) {
        total += item.getQuantity() * item.getPricePerItem();
    }
    return total;
}

void ShoppingCartDialog::handleRemoveItem()
{
    int currentRow = cartTableWidget->currentRow();
    if (currentRow < 0 || currentRow >= static_cast<int>(currentCartItems.size())) {
        QMessageBox::warning(this, "Remove Item", "Please select an item to remove.");
        return;
    }
    
    // Get the product ID from the selected row
    QTableWidgetItem* item = cartTableWidget->item(currentRow, 0);
    int productId = item->data(Qt::UserRole).toInt();
    
    // Use the backend ShoppingCart class to remove the item
    ShoppingCart cart(userId);
    if (cart.removeFromCart(productId)) {
        QMessageBox::information(this, "Remove Item", "Item removed from cart.");
        
        // Update UI
        currentCartItems.erase(currentCartItems.begin() + currentRow);
        updateCartDisplay();
    } else {
        QMessageBox::warning(this, "Error", "Failed to remove item from cart.");
    }
}

void ShoppingCartDialog::handleUpdateQuantity()
{
    int currentRow = cartTableWidget->currentRow();
    if (currentRow < 0 || currentRow >= static_cast<int>(currentCartItems.size())) {
        QMessageBox::warning(this, "Update Quantity", "Please select an item to update.");
        return;
    }
    
    // Get the product ID and new quantity
    QTableWidgetItem* item = cartTableWidget->item(currentRow, 0);
    int productId = item->data(Qt::UserRole).toInt();
    int newQuantity = quantitySpinBox->value();
    
    // Check stock availability
    Product* product = Product::getProductByID(productId);
    if (!product) {
        QMessageBox::warning(this, "Error", "Failed to verify product stock.");
        return;
    }
    
    if (newQuantity > product->getStock()) {
        QMessageBox::warning(this, "Insufficient Stock", 
                           QString("Cannot update quantity. Only %1 available in stock.")
                           .arg(product->getStock()));
        delete product;
        return;
    }
    
    delete product; // Clean up
    
    // Remove the current item and add with new quantity
    ShoppingCart cart(userId);
    if (cart.removeFromCart(productId) && cart.addToCart(productId, newQuantity)) {
        QMessageBox::information(this, "Update Quantity", "Quantity updated successfully.");
        
        // Update UI
        currentCartItems[currentRow].setQuantity(newQuantity);
        updateCartDisplay();
    } else {
        QMessageBox::warning(this, "Error", "Failed to update quantity.");
    }
}

void ShoppingCartDialog::handleProceedToCheckout()
{
    if (currentCartItems.empty()) {
        QMessageBox::information(this, "Checkout", "Your cart is empty.");
        return;
    }
    
    // Create and show the OrderSummaryDialog, passing cart items, total, and userId
    OrderSummaryDialog summaryDialog(currentCartItems, calculateTotal(), userId, this);
    if (summaryDialog.exec() == QDialog::Accepted) {
        // Order was completed, close the cart dialog
        accept();
    }
    // If rejected, we stay on this dialog (user went back to cart)
} 