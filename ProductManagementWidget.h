#ifndef PRODUCTMANAGEMENTWIDGET_H
#define PRODUCTMANAGEMENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QList>
#include <QTextStream>
#include <cstring> // For strncpy

#include "AddEditProductDialog.h"
#include "../../include/Product.h"          // Assuming backend Product struct
#include "../../include/Admin.h"            // Assuming backend Admin class with static methods

// Placeholder for Product data structure (This comment can remain or be removed)
// struct AdminProductViewItem { ... };

// Ensure NO Backend namespace or its classes/structs are defined here anymore.

class ProductManagementWidget : public QWidget
{
    Q_OBJECT

public:
    inline explicit ProductManagementWidget(QWidget *parent = nullptr);
    inline ~ProductManagementWidget();

signals:
    void productAdded(int productId); // Signal emitted when a product is added
    void productUpdated(int productId); // Signal emitted when a product is updated
    void productRemoved(int productId); // Signal emitted when a product is removed

private slots:
    inline void handleAddProduct();
    inline void handleEditProduct();
    inline void handleRemoveProduct();

private:
    inline void loadProducts();
    inline void setupUi();

    QTableWidget *productsTable;
    QPushButton *addProductButton;
    QPushButton *editProductButton;
    QPushButton *removeProductButton;
    QList<ProductDetails> displayedProductDetails;
};

inline ProductManagementWidget::ProductManagementWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadProducts();
}

inline ProductManagementWidget::~ProductManagementWidget() {}

inline void ProductManagementWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    productsTable = new QTableWidget(this);
    productsTable->setColumnCount(6); // ID, Name, Category, Price, Stock, Description (Order changed for consistency)
    productsTable->setHorizontalHeaderLabels({"ID", "Name", "Category", "Price", "Stock", "Description"});
    productsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    productsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    productsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(productsTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addProductButton = new QPushButton("Add Product", this);
    editProductButton = new QPushButton("Edit Selected Product", this);
    removeProductButton = new QPushButton("Remove Selected Product", this);
    buttonLayout->addWidget(addProductButton);
    buttonLayout->addWidget(editProductButton);
    buttonLayout->addWidget(removeProductButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(addProductButton, &QPushButton::clicked, this, &ProductManagementWidget::handleAddProduct);
    connect(editProductButton, &QPushButton::clicked, this, &ProductManagementWidget::handleEditProduct);
    connect(removeProductButton, &QPushButton::clicked, this, &ProductManagementWidget::handleRemoveProduct);
}

inline void ProductManagementWidget::loadProducts() {
    productsTable->setRowCount(0);
    // displayedProductDetails.clear(); // This list might not be needed if we fetch fresh for editing

    int productCount = 0;
    Product* backendProducts = Product::loadAllProducts(productCount); // Changed getAllProducts to loadAllProducts

    if (!backendProducts) {
        QMessageBox::critical(this, "Error Loading Products", "Could not fetch products from backend.");
        return;
    }

    productsTable->setRowCount(productCount);
    for (int i = 0; i < productCount; ++i) {
        productsTable->setItem(i, 0, new QTableWidgetItem(QString::number(backendProducts[i].getProductID()))); // Used getter
        productsTable->setItem(i, 1, new QTableWidgetItem(QString::fromUtf8(backendProducts[i].getName() ? backendProducts[i].getName() : "N/A"))); // Used getter
        productsTable->setItem(i, 2, new QTableWidgetItem(QString::fromUtf8(backendProducts[i].getCategory() ? backendProducts[i].getCategory() : "N/A"))); // Used getter
        productsTable->setItem(i, 3, new QTableWidgetItem(QString::number(backendProducts[i].getPrice(), 'f', 2))); // Used getter
        productsTable->setItem(i, 4, new QTableWidgetItem(QString::number(backendProducts[i].getStock()))); // Used getter
        const char* desc = backendProducts[i].getDescription();
        productsTable->setItem(i, 5, new QTableWidgetItem(QString::fromUtf8(desc ? desc : "N/A"))); // Used getter for description
    }
    delete[] backendProducts; // Crucial: free memory allocated by backend
}

inline void ProductManagementWidget::handleAddProduct() {
    AddEditProductDialog dialog(this); // For adding, pass no existing product
    if (dialog.exec() == QDialog::Accepted) {
        ProductDetails guiDetails = dialog.getProductDetails();
        Product backendProduct; // Instance of backend Product class

        // Convert ProductDetails (GUI) to Product (backend) using setters
        // ID is set by backend for new products.
        backendProduct.setName(guiDetails.getName().toUtf8().constData());
        backendProduct.setCategory(guiDetails.getCategory().toUtf8().constData());
        backendProduct.setPrice(guiDetails.getPrice());
        backendProduct.setStock(guiDetails.getStock());
        backendProduct.setDescription(guiDetails.getDescription().toUtf8().constData());
        // Rating is often handled by backend or based on reviews, not set directly here.
        // backendProduct.setRating(0); // Or some default if needed by Admin::addProduct

        // Call the static Product::addProduct with the Product object
        if (Product::addProduct(backendProduct)) { 
            QMessageBox::information(this, "Add Product", "Product added successfully.");
            loadProducts(); // Refresh table
            
            // Try to get the ID of the newly added product (assuming it's the last one in the table)
            if (productsTable->rowCount() > 0) {
                int newProductId = productsTable->item(productsTable->rowCount() - 1, 0)->text().toInt();
                emit productAdded(newProductId);
            }
        } else {
            QMessageBox::critical(this, "Add Product", "Failed to add product (backend error).");
        }
    }
}

inline void ProductManagementWidget::handleEditProduct() {
    QList<QTableWidgetItem*> selectedItems = productsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Edit Product", "Please select a product to edit.");
        return;
    }

    int selectedRow = selectedItems.first()->row();
    int productId = productsTable->item(selectedRow, 0)->text().toInt();

    // Fetch the full product details from backend to ensure data is fresh and complete
    Product* productToEdit = Product::getProductByID(productId);

    if (!productToEdit) {
        QMessageBox::critical(this, "Edit Product", "Could not fetch product details for editing.");
        return;
    }

    ProductDetails currentProductGui;
    currentProductGui.setId(productToEdit->getProductID());
    currentProductGui.setName(QString::fromUtf8(productToEdit->getName() ? productToEdit->getName() : ""));
    currentProductGui.setCategory(QString::fromUtf8(productToEdit->getCategory() ? productToEdit->getCategory() : ""));
    currentProductGui.setPrice(productToEdit->getPrice());
    currentProductGui.setStock(productToEdit->getStock());
    currentProductGui.setDescription(QString::fromUtf8(productToEdit->getDescription() ? productToEdit->getDescription() : ""));
    // currentProductGui.setRating(productToEdit->getRating()); // If AddEditProductDialog handles rating display

    AddEditProductDialog dialog(currentProductGui, this);
    if (dialog.exec() == QDialog::Accepted) {
        ProductDetails guiDetails = dialog.getProductDetails();
        // Create a temporary Product object to pass data to Product::editProduct
        // The Product::editProduct function should use productId to identify the product to update
        // and then use the data from this temporary product object.
        Product updatedProductData;
        updatedProductData.setName(guiDetails.getName().toUtf8().constData());
        updatedProductData.setCategory(guiDetails.getCategory().toUtf8().constData());
        updatedProductData.setPrice(guiDetails.getPrice());
        updatedProductData.setStock(guiDetails.getStock());
        updatedProductData.setDescription(guiDetails.getDescription().toUtf8().constData());
        // updatedProductData.setRating(guiDetails.getRating()); // If rating is editable and part of ProductDetails

        // Note: updatedProductData.getProductID() will be 0 here. 
        // The actual product ID for update is `productId`.
        // Call the static Product::editProduct with productId and the Product object containing new data
        if (Product::editProduct(productId, updatedProductData)) { 
            QMessageBox::information(this, "Edit Product", "Product updated successfully.");
            loadProducts(); // Refresh table
            
            // Emit the signal that a product was updated
            emit productUpdated(productId);
        } else {
            QMessageBox::critical(this, "Edit Product", "Failed to update product (backend error).");
        }
    }
    delete productToEdit; // Clean up the product fetched by getProductByID
}

inline void ProductManagementWidget::handleRemoveProduct() {
    QList<QTableWidgetItem*> selectedItems = productsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Remove Product", "Please select a product to remove.");
        return;
    }

    int selectedRow = selectedItems.first()->row();
    int productId = productsTable->item(selectedRow, 0)->text().toInt();
    QString productName = productsTable->item(selectedRow, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove Product", 
                                  QString("Are you sure you want to remove product '%1' (ID: %2)?").arg(productName).arg(productId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Call the static Product::removeProduct with productId
        if (Product::removeProduct(productId)) { 
            QMessageBox::information(this, "Remove Product", QString("Product '%1' removed successfully.").arg(productName));
            loadProducts(); // Refresh table
            
            // Emit the signal that a product was removed
            emit productRemoved(productId);
        } else {
            QMessageBox::critical(this, "Remove Product", QString("Failed to remove product '%1' (backend error).").arg(productName));
        }
    }
}

#endif // PRODUCTMANAGEMENTWIDGET_H 