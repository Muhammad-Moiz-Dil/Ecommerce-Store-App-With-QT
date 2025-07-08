#ifndef PRODUCTLISTINGWIDGET_H
#define PRODUCTLISTINGWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include "../../include/Product.h" // Backend Product class

class ProductListingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProductListingWidget(QWidget *parent = nullptr);
    ~ProductListingWidget();

signals:
    void productSelected(int productId); // Emitted when a product is selected for viewing details

public slots:
    void refreshProductList(); // Called to reload all products from data file
    void onProductAdded(int productId); // Called when a product is added elsewhere
    void onProductUpdated(int productId); // Called when a product is updated elsewhere
    void onProductRemoved(int productId); // Called when a product is removed elsewhere

private slots:
    void handleSearch(); // Search by name
    void handleFilterCategory(); // Filter by selected category
    void handleFilterPrice(); // Filter by price range
    void handleFilterRating(); // Filter by minimum rating
    void handleResetFilters(); // Reset all filters
    void handleRowDoubleClicked(const QModelIndex &index); // Handle double-click on a product

private:
    // UI Components
    QTableView *productTableView;
    QStandardItemModel *productModel;
    
    // Search components
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    
    // Filter components
    QComboBox *categoryComboBox;
    QDoubleSpinBox *minPriceSpinBox;
    QDoubleSpinBox *maxPriceSpinBox;
    QDoubleSpinBox *minRatingSpinBox;
    QPushButton *applyFilterButton;
    QPushButton *resetFilterButton;

    // Current filters
    QString currentNameFilter;
    QString currentCategoryFilter;
    double currentMinPrice;
    double currentMaxPrice;
    double currentMinRating;

    // Helper methods
    void setupUI();
    void setupConnections();
    void populateCategories(); // Load unique categories into combobox
    
    // Helper to load products based on filters
    void loadProducts(const QString &nameFilter = QString(), 
                      const QString &categoryFilter = QString(),
                      double minPrice = 0.0, 
                      double maxPrice = 10000.0, 
                      double minRating = 0.0);
                      
    // Helper to find row index by product ID
    int findRowByProductId(int productId);
    
    // Helper to update a single product row
    void updateProductRow(int productId);
};

#endif // PRODUCTLISTINGWIDGET_H 