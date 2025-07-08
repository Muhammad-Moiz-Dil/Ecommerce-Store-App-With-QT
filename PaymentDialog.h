#ifndef PAYMENTDIALOG_H
#define PAYMENTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>

// Payment class would need to be created if it doesn't exist
// #include "../../include/Payment.h"

class PaymentDialog : public QDialog
{
    Q_OBJECT

public:
    inline PaymentDialog(double amount, QWidget *parent = nullptr);
    inline ~PaymentDialog();

private slots:
    inline void handleSimulatePayment();
    inline void handlePaymentMethodChange(int index);

private:
    QLabel *amountLabel;
    QComboBox *paymentMethodComboBox;
    QPushButton *simulatePaymentButton;
    QPushButton *cancelButton;
    double paymentAmount;
    
    // Payment details widgets
    QWidget *cardDetailsWidget;
    QLineEdit *cardNumberEdit;
    QLineEdit *cardNameEdit;
    QLineEdit *cardExpiryEdit;
    QLineEdit *cardCvvEdit;
    
    QWidget *paypalDetailsWidget;
    QLineEdit *paypalEmailEdit;
    
    QWidget *giftCardDetailsWidget;
    QLineEdit *giftCardNumberEdit;
    QLineEdit *giftCardPinEdit;
};

inline PaymentDialog::PaymentDialog(double amount, QWidget *parent)
    : QDialog(parent), paymentAmount(amount)
{
    setWindowTitle("Simulate Payment");
    setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Payment amount
    amountLabel = new QLabel(QString("<b>Amount to Pay: $%1</b>").arg(paymentAmount, 0, 'f', 2), this);
    amountLabel->setAlignment(Qt::AlignCenter);
    QFont amountFont = amountLabel->font();
    amountFont.setPointSize(amountFont.pointSize() + 2);
    amountLabel->setFont(amountFont);
    mainLayout->addWidget(amountLabel);
    
    // Payment method selection
    QLabel *methodLabel = new QLabel("Select Payment Method:", this);
    methodLabel->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(methodLabel);
    
    paymentMethodComboBox = new QComboBox(this);
    paymentMethodComboBox->addItem("Credit/Debit Card");
    paymentMethodComboBox->addItem("PayPal");
    paymentMethodComboBox->addItem("Gift Card");
    mainLayout->addWidget(paymentMethodComboBox);
    
    // Credit card details widget
    cardDetailsWidget = new QWidget(this);
    QFormLayout *cardLayout = new QFormLayout(cardDetailsWidget);
    cardNumberEdit = new QLineEdit("4111 1111 1111 1111", cardDetailsWidget); // Example card number
    cardNameEdit = new QLineEdit("John Doe", cardDetailsWidget);
    cardExpiryEdit = new QLineEdit("12/25", cardDetailsWidget);
    cardCvvEdit = new QLineEdit("123", cardDetailsWidget);
    cardCvvEdit->setMaxLength(4);
    cardCvvEdit->setEchoMode(QLineEdit::Password);
    
    cardLayout->addRow("Card Number:", cardNumberEdit);
    cardLayout->addRow("Name on Card:", cardNameEdit);
    cardLayout->addRow("Expiry (MM/YY):", cardExpiryEdit);
    cardLayout->addRow("CVV:", cardCvvEdit);
    cardDetailsWidget->setLayout(cardLayout);
    mainLayout->addWidget(cardDetailsWidget);
    
    // PayPal details widget
    paypalDetailsWidget = new QWidget(this);
    QFormLayout *paypalLayout = new QFormLayout(paypalDetailsWidget);
    paypalEmailEdit = new QLineEdit("example@email.com", paypalDetailsWidget);
    paypalLayout->addRow("PayPal Email:", paypalEmailEdit);
    paypalDetailsWidget->setLayout(paypalLayout);
    paypalDetailsWidget->hide();
    mainLayout->addWidget(paypalDetailsWidget);
    
    // Gift card details widget
    giftCardDetailsWidget = new QWidget(this);
    QFormLayout *giftCardLayout = new QFormLayout(giftCardDetailsWidget);
    giftCardNumberEdit = new QLineEdit("GC12345678", giftCardDetailsWidget);
    giftCardPinEdit = new QLineEdit("1234", giftCardDetailsWidget);
    giftCardPinEdit->setEchoMode(QLineEdit::Password);
    giftCardLayout->addRow("Gift Card Number:", giftCardNumberEdit);
    giftCardLayout->addRow("PIN:", giftCardPinEdit);
    giftCardDetailsWidget->setLayout(giftCardLayout);
    giftCardDetailsWidget->hide();
    mainLayout->addWidget(giftCardDetailsWidget);
    
    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    cancelButton = new QPushButton("Cancel Payment", this);
    simulatePaymentButton = new QPushButton("Complete Payment", this);
    simulatePaymentButton->setDefault(true);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(simulatePaymentButton);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(simulatePaymentButton, &QPushButton::clicked, this, &PaymentDialog::handleSimulatePayment);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(paymentMethodComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PaymentDialog::handlePaymentMethodChange);
}

inline PaymentDialog::~PaymentDialog() {}

inline void PaymentDialog::handlePaymentMethodChange(int index) {
    // Hide all payment detail widgets
    cardDetailsWidget->hide();
    paypalDetailsWidget->hide();
    giftCardDetailsWidget->hide();
    
    // Show the selected payment method's details
    switch(index) {
        case 0: // Credit/Debit Card
            cardDetailsWidget->show();
            break;
        case 1: // PayPal
            paypalDetailsWidget->show();
            break;
        case 2: // Gift Card
            giftCardDetailsWidget->show();
            break;
    }
}

inline void PaymentDialog::handleSimulatePayment()
{
    // Get the selected payment method
    QString selectedMethod = paymentMethodComboBox->currentText();
    QString paymentDetails;
    
    // Get specific details based on the payment method
    switch(paymentMethodComboBox->currentIndex()) {
        case 0: // Credit/Debit Card
            paymentDetails = QString("Card ending in %1").arg(cardNumberEdit->text().right(4));
            break;
        case 1: // PayPal
            paymentDetails = paypalEmailEdit->text();
            break;
        case 2: // Gift Card
            paymentDetails = QString("Gift Card %1").arg(giftCardNumberEdit->text());
            break;
    }
    
    // In a real app, we would call Payment::simulatePayment() here
    // But since we're simulating, always return success
    
    QMessageBox::information(this, "Payment Successful", 
        QString("Payment of $%1 via %2 (%3) completed successfully!")
            .arg(paymentAmount, 0, 'f', 2)
            .arg(selectedMethod)
            .arg(paymentDetails));
    
    accept(); // Indicate successful payment simulation
}

#endif // PAYMENTDIALOG_H 