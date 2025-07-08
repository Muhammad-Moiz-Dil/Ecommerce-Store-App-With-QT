#ifndef INVENTORYVIEWWIDGET_H
#define INVENTORYVIEWWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel> // For a title or description
#include <QMessageBox> // For error messages if needed
#include "../../include/Admin.h"
#include "../../include/Product.h"
#include "AddEditProductDialog.h" // For ProductDetails struct

// Placeholder for Inventory data structure
// struct InventoryItemData { ... };

class InventoryViewWidget : public QWidget
{
    Q_OBJECT

public:
    inline explicit InventoryViewWidget(QWidget *parent = nullptr);
    inline ~InventoryViewWidget();
    
    inline void refreshInventory(); // Added refresh method for external updates

private slots:
    inline void handleLowStockHighlight(); // Added to highlight low stock items

private:
    inline void loadInventory();
    inline void setupUi();

    QTableWidget *inventoryTable;
    QLabel *totalItemsLabel;
    bool hasLowStock;
};

inline InventoryViewWidget::InventoryViewWidget(QWidget *parent)
    : QWidget(parent), hasLowStock(false)
{
    setupUi();
    loadInventory();
}

inline InventoryViewWidget::~InventoryViewWidget() {}

inline void InventoryViewWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QLabel *titleLabel = new QLabel("Current Product Inventory Status", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    inventoryTable = new QTableWidget(this);
    // Add Rating column that was missing before
    inventoryTable->setColumnCount(6);
    inventoryTable->setHorizontalHeaderLabels({"ID", "Name", "Category", "Price", "Rating", "Stock"});
    inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inventoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    inventoryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    inventoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    inventoryTable->setAlternatingRowColors(true);
    mainLayout->addWidget(inventoryTable);
    
    // Add total items count
    totalItemsLabel = new QLabel("Total Items: 0", this);
    totalItemsLabel->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(totalItemsLabel);
    
    // Connect selection signal for low stock highlighting
    connect(inventoryTable, &QTableWidget::itemSelectionChanged, this, &InventoryViewWidget::handleLowStockHighlight);

    setLayout(mainLayout);
}

inline void InventoryViewWidget::loadInventory() {
    inventoryTable->setRowCount(0); // Clear existing rows
    hasLowStock = false;

    int productCount = 0;
    // Use the real Product::loadAllProducts method
    Product* products = Product::loadAllProducts(productCount);

    if (!products || productCount == 0) {
        totalItemsLabel->setText("Total Items: 0");
        QMessageBox::information(this, "Inventory", "No products found in the inventory.");
        return;
    }

    inventoryTable->setRowCount(productCount);
    int lowStockCount = 0;
    
    for (int i = 0; i < productCount; ++i) {
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(products[i].getProductID()));
        QTableWidgetItem* nameItem = new QTableWidgetItem(
            products[i].getName() ? QString(products[i].getName()) : "Unnamed Product"
        );
        QTableWidgetItem* categoryItem = new QTableWidgetItem(
            products[i].getCategory() ? QString(products[i].getCategory()) : "Uncategorized"
        );
        QTableWidgetItem* priceItem = new QTableWidgetItem(
            QString("$%1").arg(products[i].getPrice(), 0, 'f', 2)
        );
        QTableWidgetItem* ratingItem = new QTableWidgetItem(
            QString("%1").arg(products[i].getRating(), 0, 'f', 1)
        );
        QTableWidgetItem* stockItem = new QTableWidgetItem(
            QString::number(products[i].getStock())
        );
        
        // Format low stock or out of stock items
        int stock = products[i].getStock();
        if (stock == 0) {
            stockItem->setBackground(QColor(255, 200, 200)); // Light red for out of stock
            stockItem->setForeground(QColor(255, 0, 0)); // Red text
            stockItem->setText(stockItem->text() + " - OUT OF STOCK!");
            hasLowStock = true;
            lowStockCount++;
        } else if (stock < 5) {
            stockItem->setBackground(QColor(255, 255, 200)); // Light yellow for low stock
            stockItem->setForeground(QColor(255, 140, 0)); // Orange text
            stockItem->setText(stockItem->text() + " - LOW STOCK!");
            hasLowStock = true;
            lowStockCount++;
        }
        
        inventoryTable->setItem(i, 0, idItem);
        inventoryTable->setItem(i, 1, nameItem);
        inventoryTable->setItem(i, 2, categoryItem);
        inventoryTable->setItem(i, 3, priceItem);
        inventoryTable->setItem(i, 4, ratingItem);
        inventoryTable->setItem(i, 5, stockItem);
    }
    
    delete[] products; // Clean up
    
    // Update the total items label including low stock warning if applicable
    QString statusText = QString("Total Items: %1").arg(productCount);
    if (lowStockCount > 0) {
        statusText += QString(" (Warning: %1 items low on stock)").arg(lowStockCount);
        totalItemsLabel->setStyleSheet("color: orange; font-weight: bold;");
    } else {
        totalItemsLabel->setStyleSheet("");
    }
    totalItemsLabel->setText(statusText);
    
    // Make sure the table columns are properly sized
    inventoryTable->resizeColumnsToContents();
}

inline void InventoryViewWidget::handleLowStockHighlight() {
    // This method helps make low stock items more visible 
    // when the user interacts with the table
    if (hasLowStock) {
        // Just reset the selection to ensure low stock highlighting is visible
        inventoryTable->clearSelection();
    }
}

inline void InventoryViewWidget::refreshInventory() {
    // Public method that can be called to refresh inventory
    loadInventory();
}

#endif // INVENTORYVIEWWIDGET_H 