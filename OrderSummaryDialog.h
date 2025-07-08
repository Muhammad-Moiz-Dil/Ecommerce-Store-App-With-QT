#ifndef ORDERSUMMARYDIALOG_H
#define ORDERSUMMARYDIALOG_H

#include <QDialog>
#include <QListWidget> // Or QTableWidget if preferred for structured item display
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <vector>
#include <QString>
// Removed include of CartTypes.h

// Direct implementation of PlaceholderCartItem instead of forward declaration
class PlaceholderCartItem {
private:
    int m_productId;
    QString m_name;
    double m_pricePerItem;
    int m_quantity;

public:
    PlaceholderCartItem() : m_productId(0), m_pricePerItem(0.0), m_quantity(0) {}
    
    PlaceholderCartItem(int productId, const QString& name, double pricePerItem, int quantity)
        : m_productId(productId), m_name(name), m_pricePerItem(pricePerItem), m_quantity(quantity) {}
    
    // Getters
    int getProductId() const { return m_productId; }
    const QString& getName() const { return m_name; }
    double getPricePerItem() const { return m_pricePerItem; }
    int getQuantity() const { return m_quantity; }
    
    // Setters
    void setProductId(int id) { m_productId = id; }
    void setName(const QString& name) { m_name = name; }
    void setPricePerItem(double price) { m_pricePerItem = price; }
    void setQuantity(int quantity) { m_quantity = quantity; }
};

#include "PaymentDialog.h" // Include the new PaymentDialog
#include "../../include/Order.h" // Include Order class for placeOrder()
#include "../../include/ShoppingCart.h" // Include ShoppingCart class for cart operations

class OrderSummaryDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor will take the list of cart items, the total, and the user ID
    inline explicit OrderSummaryDialog(const std::vector<PlaceholderCartItem>& items, double total, int userId, QWidget *parent = nullptr);
    inline ~OrderSummaryDialog();

signals:
    void orderPlaced(int orderId);  // Signal emitted when an order is successfully placed
    void orderStatusUpdated(int orderId, const QString& newStatus); // Signal emitted when an order status is updated

private slots:
    inline void handleProceedToPayment();

private:
    QListWidget *itemsListWidget; // To display item summaries
    QLabel *totalLabel;
    QPushButton *proceedButton;
    QPushButton *cancelButton;
    double currentOrderTotal; // Member to store the total
    std::vector<PlaceholderCartItem> currentOrderItems; // Store items
    int currentUserId; // Store the user ID for order placement
};

inline OrderSummaryDialog::OrderSummaryDialog(const std::vector<PlaceholderCartItem>& items, double total, int userId, QWidget *parent)
    : QDialog(parent), currentOrderTotal(total), currentOrderItems(items), currentUserId(userId)
{
    setWindowTitle("Order Summary & Confirmation");
    setMinimumWidth(450);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("<b>Please review your order:</b>", this));

    itemsListWidget = new QListWidget(this);
    for (const auto& item : items) {
        itemsListWidget->addItem(QString("%1 x %2 (Price: $%3 each) - Subtotal: $%4")
            .arg(item.getQuantity())
            .arg(item.getName())
            .arg(item.getPricePerItem(), 0, 'f', 2)
            .arg(item.getQuantity() * item.getPricePerItem(), 0, 'f', 2));
    }
    itemsListWidget->setFixedHeight(150); // Adjust as needed
    mainLayout->addWidget(itemsListWidget);

    totalLabel = new QLabel(QString("<b>Order Total: $%1</b>").arg(total, 0, 'f', 2), this);
    QFont totalFont = totalLabel->font();
    totalFont.setPointSize(totalFont.pointSize() + 2);
    totalLabel->setFont(totalFont);
    totalLabel->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(totalLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    cancelButton = new QPushButton("Back to Cart / Cancel", this);
    proceedButton = new QPushButton("Confirm & Proceed to Payment", this);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(proceedButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(proceedButton, &QPushButton::clicked, this, &OrderSummaryDialog::handleProceedToPayment);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject); // User cancels order confirmation
}

inline OrderSummaryDialog::~OrderSummaryDialog() {}

inline void OrderSummaryDialog::handleProceedToPayment()
{
    PaymentDialog paymentDialog(currentOrderTotal, this->parentWidget()); 
    if (paymentDialog.exec() == QDialog::Accepted) {
        // Payment was simulated successfully
        // Get ShoppingCart for current user
        ShoppingCart cart(currentUserId);
        
        // Create an Order object with the user ID directly in the constructor
        Order order(0, currentUserId, "", nullptr, "Pending");
        
        // Place the order using the Order::placeOrder method
        int orderId = order.placeOrder(cart);
        
        if (orderId > 0) {
            // Order was placed successfully (orderId is the new order ID)
            QMessageBox::information(this, "Order Successful", 
                QString("Your order has been placed successfully!\nOrder ID: %1\nOrder Total: $%2")
                .arg(orderId)
                .arg(currentOrderTotal, 0, 'f', 2));
                
            // Clear the cart (it was already cleared by placeOrder, but just to be safe)
            cart.clearCart();
            
            // Emit the orderPlaced signal
            emit orderPlaced(orderId);
            
            accept(); // Close OrderSummary, signaling to cart to also close as checkout is complete
        } else {
            QMessageBox::critical(this, "Order Failed", 
                "There was an issue placing your order. Please try again later.\n"
                "Possible reasons:\n"
                "- Some items may be out of stock\n"
                "- Technical error in processing the order");
        }
    } else {
        // Payment was cancelled
        QMessageBox::warning(this, "Payment Cancelled", "Payment was cancelled. Your order has not been placed.");
    }
}

#endif // ORDERSUMMARYDIALOG_H 