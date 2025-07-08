#ifndef UPDATEORDERSTATUSDIALOG_H
#define UPDATEORDERSTATUSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QString>

#include "../../include/Order.h" // Use Order class directly

class UpdateOrderStatusDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor takes the current order ID and status
    inline explicit UpdateOrderStatusDialog(int orderId, const QString &currentStatus, QWidget *parent = nullptr);
    inline ~UpdateOrderStatusDialog();

    inline QString getNewStatus() const;

private:
    inline void setupUi(int orderId, const QString &currentStatus);

    QLabel *orderIdLabel;
    QComboBox *statusComboBox;
    QDialogButtonBox *buttonBox;
};

inline UpdateOrderStatusDialog::UpdateOrderStatusDialog(int orderId, const QString &currentStatus, QWidget *parent)
    : QDialog(parent)
{
    setupUi(orderId, currentStatus);
    setWindowTitle("Update Order Status");
}

inline UpdateOrderStatusDialog::~UpdateOrderStatusDialog() {}

inline void UpdateOrderStatusDialog::setupUi(int orderId, const QString &currentStatus) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    orderIdLabel = new QLabel(QString::number(orderId), this);
    statusComboBox = new QComboBox(this);
    // Example statuses. In a real app, these might come from a config or backend.
    statusComboBox->addItems({"Processing", "Shipped", "Delivered", "Cancelled", "Pending Payment", "Awaiting Pickup"});
    
    if (statusComboBox->findText(currentStatus) != -1) {
        statusComboBox->setCurrentText(currentStatus);
    } else if (statusComboBox->count() > 0) {
        statusComboBox->setCurrentIndex(0); // Default to first if current not found
    }

    formLayout->addRow("Order ID:", orderIdLabel);
    formLayout->addRow("New Status:", statusComboBox);

    mainLayout->addLayout(formLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setMinimumWidth(300);
}

inline QString UpdateOrderStatusDialog::getNewStatus() const {
    return statusComboBox->currentText();
}

#endif // UPDATEORDERSTATUSDIALOG_H 