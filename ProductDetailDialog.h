#ifndef PRODUCTDETAILDIALOG_H
#define PRODUCTDETAILDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QTabWidget>
#include "../../include/Product.h"
#include "ReviewWidget.h"

class ProductDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDetailDialog(int productId, bool isAuthenticated, int userId = 0, QWidget *parent = nullptr);
    ~ProductDetailDialog();

signals:
    void addToCartRequested(int productId, int quantity);
    void addToWishlistRequested(int productId);
    void reviewAdded(int productId);
    void reviewAddedByUser(int userId);

private slots:
    void handleAddToCart();
    void handleAddToWishlist();

private:
    void setupUI();
    void loadProductData();
    
    int productId;
    int userId;
    bool isAuthenticated;
    Product* productData;
    
    // UI elements
    QLabel *nameLabel;
    QLabel *categoryLabel;
    QLabel *priceLabel;
    QLabel *ratingLabel;
    QLabel *stockLabel;
    QLabel *descriptionLabel;
    
    QSpinBox *quantitySpinBox;
    QPushButton *addToCartButton;
    QPushButton *addToWishlistButton;
    QPushButton *closeButton;
    
    // Tabs
    QTabWidget *tabWidget;
    ReviewWidget *reviewWidget;
};

#endif // PRODUCTDETAILDIALOG_H 