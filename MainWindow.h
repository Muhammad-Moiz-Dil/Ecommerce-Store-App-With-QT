#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QMessageBox>
#include <QLabel> // For a welcome message or status
#include <QTimer> // For QTimer::singleShot

// Forward declarations for dialogs to avoid circular dependencies if they include MainWindow.h
class LoginDialog;
class RegistrationDialog;
class ProductListingWidget;
class ProductDetailDialog;
class ShoppingCartDialog;
class UserDashboardDialog;
class AdminDashboardDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    // Signals for selective data updates
    void productAdded(int productId);
    void productUpdated(int productId);
    void productRemoved(int productId);
    
    void cartItemAdded(int userId, int productId);
    void cartItemRemoved(int userId, int productId);
    void cartItemQuantityChanged(int userId, int productId);
    void cartCleared(int userId);
    
    void orderPlaced(int userId, int orderId);
    void orderStatusChanged(int orderId, const QString& newStatus);
    
    void wishlistItemAdded(int userId, int productId);
    void wishlistItemRemoved(int userId, int productId);
    
    void userAdded(int userId);
    void userUpdated(int userId);
    void userRemoved(int userId);
    
    void reviewAdded(int productId, int userId);

protected:
    // void showEvent(QShowEvent *event) override; // To show login dialog on startup

private slots:
    // Authentication slots
    void showLoginDialog();
    void showRegisterDialog();
    void onUserLoggedIn(int userId, const QString& username, bool isAdmin);
    void onUserRegistered(int userId, const QString& username, bool isAdmin);
    void handleSwitchToRegister();
    void handleSwitchToLogin();
    void handleLogout(); // New slot for logout function
    
    // Navigation slots
    void viewProducts();
    void showShoppingCartDialog();
    void showUserDashboardDialog();
    void adminDashboard();
    
    // Product interaction slots
    void showProductDetails(int productId);
    void handleAddToCart(int productId, int quantity);
    void handleAddToWishlist(int productId);
    
    // Data change slots
    void onProductDataChanged();
    void onCartDataChanged();
    void onOrderDataChanged();
    void onWishlistDataChanged();
    void onUserDataChanged();
    void onReviewDataChanged();

private:
    void setupUI();
    void createMenusAndToolbar(); // Optional: for menu bar/toolbar as per gui_plan.md
    void refreshViews(); // Helper to refresh UI components after data changes
    void initDataDirectories(); // Initialize data directories if they don't exist

    QAction *loginAction;
    QAction *registerAction;
    QAction *viewProductsAction;
    QAction *exitAction;
    QAction *viewCartAction;
    QAction *userDashboardAction;
    QAction *adminDashboardAction;
    QAction *logoutAction; // New action for logout button

    LoginDialog *loginDialog;
    RegistrationDialog *registrationDialog;

    QLabel *welcomeLabel;
    QLabel *statusLabel;
    
    ProductListingWidget *productListingWidget;
    
    int currentUserId;
    QString currentUsername;
    bool isAuthenticated;
    bool isAdminUser;
};

#endif // MAINWINDOW_H