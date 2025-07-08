#ifndef ADMINDASHBOARDDIALOG_H
#define ADMINDASHBOARDDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>

// Forward declarations for actual widgets to be implemented later
// class ProductManagementWidget;
// class UserManagementWidget;
// class OrderManagementWidget;
// class InventoryViewWidget;
#include "ProductManagementWidget.h"
#include "UserManagementWidget.h"
#include "OrderManagementWidget.h"
#include "InventoryViewWidget.h"

class AdminDashboardDialog : public QDialog
{
    Q_OBJECT

public:
    inline explicit AdminDashboardDialog(QWidget *parent = nullptr);
    inline ~AdminDashboardDialog();

signals:
    void productAdded(int productId); // Signal emitted when a product is added
    void productUpdated(int productId); // Signal emitted when a product is updated
    void productRemoved(int productId); // Signal emitted when a product is removed
    void userUpdated(int userId); // Signal emitted when a user is updated
    void userRemoved(int userId); // Signal emitted when a user is removed
    void orderStatusUpdated(int orderId, const QString& newStatus); // Signal emitted when an order status is updated

private:
    QTabWidget *tabWidget;

    // Pointers to actual widgets will be added here later
    // ProductManagementWidget *productManagementTab;
    // UserManagementWidget *userManagementTab;
    // OrderManagementWidget *orderManagementTab;
    // InventoryViewWidget *inventoryViewTab;
    ProductManagementWidget *productManagementWidget;
    UserManagementWidget *userManagementWidget;
    OrderManagementWidget *orderManagementWidget;
    InventoryViewWidget *inventoryViewWidget;
};

inline AdminDashboardDialog::AdminDashboardDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Admin Dashboard");
    setMinimumSize(800, 600); // Admin dashboard might need more space

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    tabWidget = new QTabWidget(this);

    // Placeholder Tab 1: Product Management
    // QWidget *productMgmtPlaceholder = new QWidget();
    // QVBoxLayout *productMgmtLayout = new QVBoxLayout(productMgmtPlaceholder);
    // productMgmtLayout->addWidget(new QLabel("Product Management (Content Coming Soon)", productMgmtPlaceholder));
    // productMgmtPlaceholder->setLayout(productMgmtLayout);
    // tabWidget->addTab(productMgmtPlaceholder, "Product Management");
    productManagementWidget = new ProductManagementWidget(this);
    tabWidget->addTab(productManagementWidget, "Product Management");

    // Connect product management signals
    connect(productManagementWidget, &ProductManagementWidget::productAdded, this, &AdminDashboardDialog::productAdded);
    connect(productManagementWidget, &ProductManagementWidget::productUpdated, this, &AdminDashboardDialog::productUpdated);
    connect(productManagementWidget, &ProductManagementWidget::productRemoved, this, &AdminDashboardDialog::productRemoved);

    // Placeholder Tab 2: User Management
    // QWidget *userMgmtPlaceholder = new QWidget();
    // QVBoxLayout *userMgmtLayout = new QVBoxLayout(userMgmtPlaceholder);
    // userMgmtLayout->addWidget(new QLabel("User Management (Content Coming Soon)", userMgmtPlaceholder));
    // userMgmtPlaceholder->setLayout(userMgmtLayout);
    // tabWidget->addTab(userMgmtPlaceholder, "User Management");
    userManagementWidget = new UserManagementWidget(this);
    tabWidget->addTab(userManagementWidget, "User Management");

    // Connect user management signals
    connect(userManagementWidget, &UserManagementWidget::userUpdated, this, &AdminDashboardDialog::userUpdated);
    connect(userManagementWidget, &UserManagementWidget::userRemoved, this, &AdminDashboardDialog::userRemoved);

    // Placeholder Tab 3: Order Management
    // QWidget *orderMgmtPlaceholder = new QWidget();
    // QVBoxLayout *orderMgmtLayout = new QVBoxLayout(orderMgmtPlaceholder);
    // orderMgmtLayout->addWidget(new QLabel("Order Management (Content Coming Soon)", orderMgmtPlaceholder));
    // orderMgmtPlaceholder->setLayout(orderMgmtLayout);
    // tabWidget->addTab(orderMgmtPlaceholder, "Order Management");
    orderManagementWidget = new OrderManagementWidget(this);
    tabWidget->addTab(orderManagementWidget, "Order Management");
    
    // Placeholder Tab 4: Inventory View
    // QWidget *inventoryPlaceholder = new QWidget();
    // QVBoxLayout *inventoryLayout = new QVBoxLayout(inventoryPlaceholder);
    // inventoryLayout->addWidget(new QLabel("Inventory View (Content Coming Soon)", inventoryPlaceholder));
    // inventoryPlaceholder->setLayout(inventoryLayout);
    // tabWidget->addTab(inventoryPlaceholder, "Inventory View");
    inventoryViewWidget = new InventoryViewWidget(this);
    tabWidget->addTab(inventoryViewWidget, "Inventory View");

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

inline AdminDashboardDialog::~AdminDashboardDialog() {}

#endif // ADMINDASHBOARDDIALOG_H 