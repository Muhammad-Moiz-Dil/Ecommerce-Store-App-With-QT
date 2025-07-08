#ifndef USERDASHBOARDDIALOG_H
#define USERDASHBOARDDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include "UserProfileWidget.h"   // For user profile tab
#include "OrderHistoryWidget.h"  // For order history tab
#include "WishlistWidget.h"      // For wishlist tab

class UserDashboardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserDashboardDialog(int userId, const QString& username, QWidget *parent = nullptr);
    ~UserDashboardDialog();
    
    void refreshAllData(); // Method to refresh all data in all tabs

signals:
    void productSelected(int productId); // To show product details
    void userProfileUpdated(); // Signal emitted when the user profile is updated
    void wishlistUpdated(); // Signal emitted when the wishlist is updated
    void orderStatusViewed(int orderId); // Signal emitted when an order status is viewed
    void orderStatusUpdated(int orderId, const QString& newStatus); // Signal emitted when an order status is updated

private slots:
    void handleProductSelection(int productId);

private:
    void setupUI();
    
    QTabWidget *tabWidget;
    UserProfileWidget *profileWidget;
    OrderHistoryWidget *orderHistoryWidget;
    WishlistWidget *wishlistWidget;
    
    int userId;
    QString username;
};

#endif // USERDASHBOARDDIALOG_H 