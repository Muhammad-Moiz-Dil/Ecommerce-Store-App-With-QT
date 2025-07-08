#ifndef WISHLISTWIDGET_H
#define WISHLISTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "../../include/Wishlist.h"
#include "../../include/Product.h"
#include "../../include/ShoppingCart.h"

class WishlistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WishlistWidget(int userId, QWidget *parent = nullptr);
    ~WishlistWidget();
    
    void refreshWishlist(); // Call this to reload wishlist after changes

signals:
    void productSelected(int productId); // Same signal as ProductListingWidget

private slots:
    void handleRemoveFromWishlist();
    void handleMoveToCart();
    void handleProductDoubleClicked(int row, int column);

private:
    void setupUI();
    void loadWishlistItems();
    
    QTableWidget *wishlistTableWidget;
    QPushButton *removeButton;
    QPushButton *moveToCartButton;
    
    int userId;
};

#endif // WISHLISTWIDGET_H 