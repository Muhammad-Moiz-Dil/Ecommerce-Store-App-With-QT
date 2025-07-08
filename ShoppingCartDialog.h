#ifndef SHOPPINGCARTDIALOG_H
#define SHOPPINGCARTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSpinBox>
#include <vector>
#include <QString>

// Include OrderSummaryDialog.h which now contains PlaceholderCartItem
#include "OrderSummaryDialog.h"
#include "../../include/ShoppingCart.h"   // For backend ShoppingCart class
#include "../../include/Product.h"        // For backend Product class

class ShoppingCartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShoppingCartDialog(int userId, QWidget *parent = nullptr);
    ~ShoppingCartDialog();

signals:
    void cartUpdated(); // Signal emitted when the cart is updated (item removed, quantity changed)
    void orderPlaced(int orderId); // Signal emitted when an order is successfully placed
    void orderStatusUpdated(int orderId, const QString& newStatus); // Signal emitted when an order status is updated

private slots:
    void handleRemoveItem();
    void handleUpdateQuantity();
    void handleProceedToCheckout();

private:
    void setupUI();
    void loadCartItems();
    void updateCartDisplay();
    double calculateTotal();

    QTableWidget *cartTableWidget;
    QLabel *totalLabel;
    QPushButton *removeButton;
    QPushButton *updateButton;
    QPushButton *checkoutButton;
    QPushButton *closeButton;
    QSpinBox *quantitySpinBox;

    int userId;
    std::vector<PlaceholderCartItem> currentCartItems;
};

#endif // SHOPPINGCARTDIALOG_H 