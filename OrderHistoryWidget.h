#ifndef ORDERHISTORYWIDGET_H
#define ORDERHISTORYWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "../../include/Order.h" // For backend Order class

class OrderHistoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderHistoryWidget(int userId, QWidget *parent = nullptr);
    ~OrderHistoryWidget();
    
    void loadOrderHistory(); // Moved to public to allow refresh from outside

private slots:
    void handleViewOrderDetails();
    void handleTrackOrder();
    void handleRefreshOrders();

private:
    void setupUI();
    
    QTableWidget *ordersTableWidget;
    QPushButton *viewDetailsButton;
    QPushButton *trackOrderButton;
    QPushButton *refreshButton;
    
    int userId;
};

#endif // ORDERHISTORYWIDGET_H 