#ifndef USERPROFILEWIDGET_H
#define USERPROFILEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout> // For neatly aligning labels and fields
#include <QGroupBox>
#include <QMessageBox>
#include <QString>
#include "../../include/User.h" // For backend User class

// Placeholder for User class interaction
// #include "../src/User.h"

class UserProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserProfileWidget(int userId, const QString& username, QWidget *parent = nullptr);
    ~UserProfileWidget();
    
    void loadUserData(); // Moved to public to allow refresh from outside

private slots:
    void handleUpdateEmail();
    void handleUpdatePassword();

private:
    void setupUI();
    
    QLabel *usernameLabel;
    QLabel *emailLabel;
    QLineEdit *newEmailEdit;
    QLineEdit *currentPasswordEdit;
    QLineEdit *newPasswordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *updateEmailButton;
    QPushButton *updatePasswordButton;
    
    int userId;
    QString username;
    QString email;
};

#endif // USERPROFILEWIDGET_H 