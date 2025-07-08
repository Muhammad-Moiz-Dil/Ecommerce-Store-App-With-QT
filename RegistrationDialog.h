#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
// Forward declare layouts if they are only used in .cpp
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QMessageBox>

// Forward declaration for User.h is not needed here if only called in .cpp
// // #include "../src/User.h" 

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();

signals:
    void userRegistered(int userId, const QString& username, bool isAdmin);
    void switchToLoginDialog(); // Signal to switch back to the login dialog

private slots:
    void handleRegistration();
    void handleSwitchToLogin();

private:
    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QLabel *emailLabel;
    QLineEdit *emailLineEdit;
    QPushButton *registerButton;
    QPushButton *switchToLoginButton;

    void setupUI(); // Declaration of UI setup method
};

#endif // REGISTRATIONDIALOG_H 