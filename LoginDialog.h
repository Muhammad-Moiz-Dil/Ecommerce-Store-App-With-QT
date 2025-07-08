#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

// Forward declaration if needed by MainWindow or other components
// class MainWindow;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void userLoggedIn(int userId, const QString& username, bool isAdmin); // Updated signal with more info
    void switchToRegisterDialog();      // Signal to switch to the registration dialog

private slots:
    void handleLogin();
    void handleSwitchToRegister();

private:
    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QPushButton *switchToRegisterButton;

    void setupUI();
};

#endif // LOGINDIALOG_H 