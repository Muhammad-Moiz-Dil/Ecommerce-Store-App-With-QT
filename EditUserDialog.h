#ifndef EDITUSERDIALOG_H
#define EDITUSERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QString>

// Forward declaration
class UserDetails;

class EditUserDialog : public QDialog
{
    Q_OBJECT

public:
    inline explicit EditUserDialog(const UserDetails &userDetails, QWidget *parent = nullptr);
    inline ~EditUserDialog();

    inline UserDetails getUserDetails() const;

private:
    inline void setupUi(const UserDetails &userDetails);

    QLabel *userIdLabel;
    QLabel *usernameLabel;
    QLineEdit *emailLineEdit;
    QComboBox *roleComboBox;
    QDialogButtonBox *buttonBox;

    UserDetails* currentUserDetails; // To store initial details and modifications
};

// Include the actual definition now
#include "UserManagementWidget.h"

inline EditUserDialog::EditUserDialog(const UserDetails &userDetails, QWidget *parent)
    : QDialog(parent)
{
    currentUserDetails = new UserDetails(userDetails);
    setupUi(userDetails);
    setWindowTitle("Edit User");
}

inline EditUserDialog::~EditUserDialog() {
    delete currentUserDetails;
}

inline void EditUserDialog::setupUi(const UserDetails &userDetails) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    userIdLabel = new QLabel(QString::number(userDetails.getId()), this);
    usernameLabel = new QLabel(userDetails.getUsername(), this);
    emailLineEdit = new QLineEdit(userDetails.getEmail(), this);
    
    roleComboBox = new QComboBox(this);
    roleComboBox->addItems({"Customer", "Admin"}); // Define available roles
    if (roleComboBox->findText(userDetails.getRole()) != -1) {
        roleComboBox->setCurrentText(userDetails.getRole());
    } else if (roleComboBox->count() > 0) {
        roleComboBox->setCurrentIndex(0); // Default to first if current not found
    }

    formLayout->addRow("User ID:", userIdLabel);
    formLayout->addRow("Username:", usernameLabel);
    formLayout->addRow("Email:", emailLineEdit);
    formLayout->addRow("Role:", roleComboBox);

    mainLayout->addLayout(formLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setMinimumWidth(350);
}

inline UserDetails EditUserDialog::getUserDetails() const {
    UserDetails updatedDetails = *currentUserDetails; // Start with existing non-editable details
    updatedDetails.setEmail(emailLineEdit->text());
    updatedDetails.setRole(roleComboBox->currentText());
    return updatedDetails;
}

#endif // EDITUSERDIALOG_H 