#ifndef ORDERMANAGEMENTWIDGET_H
#define ORDERMANAGEMENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <sstream>
#include <string>
#include <fstream>
#include "UpdateOrderStatusDialog.h"
#include "../../include/Order.h" // Include actual Order class

class OrderManagementWidget : public QWidget
{
    Q_OBJECT

public:
    inline explicit OrderManagementWidget(QWidget *parent = nullptr);
    inline ~OrderManagementWidget();

private slots:
    inline void handleUpdateStatus();
    inline void handleRefresh(); // New method to refresh order list

private:
    inline void loadOrders(); // Modified to use actual Order class
    inline void setupUi();
    inline double calculateOrderTotal(const char* orderItems); // Helper to calculate order total

    QTableWidget *ordersTable;
    QPushButton *updateStatusButton;
    QPushButton *refreshButton; // New refresh button
};

inline OrderManagementWidget::OrderManagementWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    loadOrders(); // Load orders when the widget is created
}

inline OrderManagementWidget::~OrderManagementWidget() {}

inline void OrderManagementWidget::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    ordersTable = new QTableWidget(this);
    ordersTable->setColumnCount(5); // ID, UserID, Date, Total, Status
    ordersTable->setHorizontalHeaderLabels({"Order ID", "User ID", "Date", "Total", "Status"});
    ordersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ordersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ordersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(ordersTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    updateStatusButton = new QPushButton("Update Selected Order Status", this);
    refreshButton = new QPushButton("Refresh Orders", this);
    buttonLayout->addWidget(updateStatusButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(updateStatusButton, &QPushButton::clicked, this, &OrderManagementWidget::handleUpdateStatus);
    connect(refreshButton, &QPushButton::clicked, this, &OrderManagementWidget::handleRefresh);
}

inline double OrderManagementWidget::calculateOrderTotal(const char* orderItems) {
    if (!orderItems) return 0.0;
    
    double total = 0.0;
    std::string items(orderItems);
    std::stringstream ss(items);
    std::string item;
    
    // Format: "productID1:qty1|productID2:qty2|..."
    while (std::getline(ss, item, '|')) {
        std::stringstream itemStream(item);
        std::string idStr, qtyStr;
        if (std::getline(itemStream, idStr, ':') && std::getline(itemStream, qtyStr)) {
            try {
                int productId = std::stoi(idStr);
                int quantity = std::stoi(qtyStr);
                
                // Get product price from Product class
                Product* product = Product::getProductByID(productId);
                if (product) {
                    total += product->getPrice() * quantity;
                    delete product;
                }
            } catch (...) {
                // Skip invalid entries
            }
        }
    }
    
    return total;
}

inline void OrderManagementWidget::loadOrders() {
    ordersTable->setRowCount(0); // Clear existing rows
    
    // Read orders directly from the file
    std::ifstream inFile("data/orders/orders.txt");
    if (!inFile.is_open()) {
        QMessageBox::warning(this, "Order Management", "No orders found or could not open orders file.");
        return;
    }
    
    std::string line;
    int row = 0;
    
    // First count the number of rows to set the table size
    int orderCount = 0;
    while (std::getline(inFile, line)) {
        if (!line.empty()) orderCount++;
    }
    
    // Reset file position to beginning
    inFile.clear();
    inFile.seekg(0, std::ios::beg);
    
    if (orderCount == 0) {
        QMessageBox::information(this, "Order Management", "No orders found in the system.");
        inFile.close();
        return;
    }
    
    ordersTable->setRowCount(orderCount);
    
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string oid_str, uid_str, items_str, date_str, status_str;
        
        std::getline(ss, oid_str, ',');
        std::getline(ss, uid_str, ',');
        std::getline(ss, items_str, ',');
        std::getline(ss, date_str, ',');
        std::getline(ss, status_str, ',');
        
        try {
            int orderId = std::stoi(oid_str);
            int userId = std::stoi(uid_str);
            
            // Calculate order total from items
            double total = calculateOrderTotal(items_str.c_str());
            
            // Set table items
            ordersTable->setItem(row, 0, new QTableWidgetItem(QString::number(orderId)));
            ordersTable->setItem(row, 1, new QTableWidgetItem(QString::number(userId)));
            ordersTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(date_str)));
            ordersTable->setItem(row, 3, new QTableWidgetItem(QString::number(total, 'f', 2)));
            ordersTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(status_str)));
            
            row++;
        } catch (...) {
            // Skip invalid entries
        }
    }
    
    inFile.close();
}

inline void OrderManagementWidget::handleUpdateStatus() {
    QList<QTableWidgetItem*> selectedItems = ordersTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Update Status", "Please select an order to update.");
        return;
    }

    int selectedRow = selectedItems.first()->row();
    int orderId = ordersTable->item(selectedRow, 0)->text().toInt();
    QString currentStatus = ordersTable->item(selectedRow, 4)->text();

    UpdateOrderStatusDialog dialog(orderId, currentStatus, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString newStatus = dialog.getNewStatus();
        // Call the actual Order class method
        if (Order::updateStatus(orderId, newStatus.toStdString().c_str())) {
            QMessageBox::information(this, "Update Status", "Order status updated successfully.");
            loadOrders(); // Refresh the table
        } else {
            QMessageBox::critical(this, "Update Status", "Failed to update order status.");
        }
    }
}

inline void OrderManagementWidget::handleRefresh() {
    loadOrders();
}

#endif // ORDERMANAGEMENTWIDGET_H 