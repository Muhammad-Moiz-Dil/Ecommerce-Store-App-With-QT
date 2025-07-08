#include "../include/UserDashboardDialog.h"
#include <QMessageBox>

UserDashboardDialog::UserDashboardDialog(int userId, const QString& username, QWidget *parent)
    : QDialog(parent), userId(userId), username(username)
{
    setWindowTitle("User Dashboard - " + username);
    setMinimumSize(700, 500);
    
    setupUI();
    
    // Connect signals from the wishlist widget to handle product selection
    connect(wishlistWidget, &WishlistWidget::productSelected, this, &UserDashboardDialog::handleProductSelection);
    // We would also connect similar signals from other tabs if they emit productSelected
    
    // Ensure all tabs show the latest data
    refreshAllData();
}

UserDashboardDialog::~UserDashboardDialog()
{
    // Qt handles deletion of child widgets
}

void UserDashboardDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    tabWidget = new QTabWidget(this);
    
    // Tab 1: Profile
    profileWidget = new UserProfileWidget(userId, username, this);
    tabWidget->addTab(profileWidget, "My Profile");
    
    // Tab 2: Order History
    orderHistoryWidget = new OrderHistoryWidget(userId, this);
    tabWidget->addTab(orderHistoryWidget, "My Orders");
    
    // Tab 3: Wishlist
    wishlistWidget = new WishlistWidget(userId, this);
    tabWidget->addTab(wishlistWidget, "My Wishlist");
    
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

void UserDashboardDialog::handleProductSelection(int productId)
{
    // Forward the signal to the MainWindow
    emit productSelected(productId);
}

void UserDashboardDialog::refreshAllData()
{
    // Refresh data in each tab
    profileWidget->loadUserData();
    orderHistoryWidget->loadOrderHistory();
    wishlistWidget->refreshWishlist();
} 