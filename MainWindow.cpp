#include "gui/include/MainWindow.h"
#include "gui/include/LoginDialog.h"
#include "gui/include/RegistrationDialog.h"
#include "gui/include/ProductListingWidget.h"
#include "gui/include/ProductDetailDialog.h"
#include "gui/include/ShoppingCartDialog.h"
#include "gui/include/UserDashboardDialog.h"
#include "gui/include/AdminDashboardDialog.h"
#include "include/Product.h"
#include "include/ShoppingCart.h"
#include "include/Wishlist.h"

#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QFileInfo>
#include <QDir>
#include <fstream>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      loginAction(nullptr),
      registerAction(nullptr),
      viewProductsAction(nullptr),
      exitAction(nullptr),
      viewCartAction(nullptr),
      userDashboardAction(nullptr),
      adminDashboardAction(nullptr),
      loginDialog(nullptr),
      registrationDialog(nullptr),
      welcomeLabel(nullptr),
      statusLabel(nullptr),
      productListingWidget(nullptr),
      currentUserId(0),
      isAuthenticated(false),
      isAdminUser(false)
{
    setupUI();
    createMenusAndToolbar();
    initDataDirectories();
}

MainWindow::~MainWindow() {
    delete loginDialog;
    delete registrationDialog;
}

void MainWindow::setupUI() {
    setWindowTitle("E-Commerce Application");
    setMinimumSize(800, 600);

    welcomeLabel = new QLabel("Please login or register to continue.", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont welcomeFont = welcomeLabel->font();
    welcomeFont.setPointSize(18);
    welcomeLabel->setFont(welcomeFont);

    statusLabel = new QLabel("Ready", this);
    statusBar()->addWidget(statusLabel);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->addWidget(welcomeLabel);
    centralLayout->addStretch();
    setCentralWidget(centralWidget);
}

void MainWindow::createMenusAndToolbar() {
    loginAction = new QAction(tr("&Login"), this);
    registerAction = new QAction(tr("&Register"), this);
    viewProductsAction = new QAction(tr("&View Products"), this);
    exitAction = new QAction(tr("E&xit"), this);
    viewCartAction = new QAction(tr("&View Cart"), this);
    userDashboardAction = new QAction(tr("&User Dashboard"), this);
    adminDashboardAction = new QAction(tr("&Admin Dashboard"), this);
    logoutAction = new QAction(tr("&Logout"), this);

    connect(loginAction, &QAction::triggered, this, &MainWindow::showLoginDialog);
    connect(registerAction, &QAction::triggered, this, &MainWindow::showRegisterDialog);
    connect(viewProductsAction, &QAction::triggered, this, &MainWindow::viewProducts);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit); // qApp is a global ptr to QApplication
    connect(viewCartAction, &QAction::triggered, this, &MainWindow::showShoppingCartDialog);
    connect(userDashboardAction, &QAction::triggered, this, &MainWindow::showUserDashboardDialog);
    connect(adminDashboardAction, &QAction::triggered, this, &MainWindow::adminDashboard);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::handleLogout);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loginAction);
    fileMenu->addAction(registerAction);
    fileMenu->addAction(viewProductsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QMenu *userMenu = menuBar()->addMenu(tr("&User"));
    userMenu->addAction(viewCartAction);
    userMenu->addAction(userDashboardAction);
    userMenu->addSeparator();
    userMenu->addAction(logoutAction);

    QMenu *adminMenu = menuBar()->addMenu(tr("&Admin"));
    adminMenu->addAction(adminDashboardAction);


    viewCartAction->setEnabled(false);
    userDashboardAction->setEnabled(false);
    adminDashboardAction->setEnabled(false);
    logoutAction->setEnabled(false);
}

void MainWindow::initDataDirectories() {
    QDir dataDir;
    
    if (!dataDir.exists("data")) {
        dataDir.mkdir("data");
    }
    
    if (!dataDir.exists("data/cart")) {
        dataDir.mkdir("data/cart");
    }
    
    if (!dataDir.exists("data/wishlist")) {
        dataDir.mkdir("data/wishlist");
    }
    
    if (!dataDir.exists("data/orders")) {
        dataDir.mkdir("data/orders");
    }
    
    if (!dataDir.exists("data/reviews")) {
        dataDir.mkdir("data/reviews");
    }
}

void MainWindow::refreshViews() {
    
    if (productListingWidget) {
        productListingWidget->refreshProductList();
    }
    
    statusLabel->setText("Data refreshed");
    QTimer::singleShot(3000, [this]() {
        statusLabel->setText("Ready");
    });
}

void MainWindow::showLoginDialog() {
    if (!loginDialog) {
        loginDialog = new LoginDialog(this);
        connect(loginDialog, &LoginDialog::userLoggedIn, this, &MainWindow::onUserLoggedIn);
        connect(loginDialog, &LoginDialog::switchToRegisterDialog, this, &MainWindow::handleSwitchToRegister);
    }
    loginDialog->exec();
}

void MainWindow::showRegisterDialog() {
    if (!registrationDialog) {
        registrationDialog = new RegistrationDialog(this);
        connect(registrationDialog, &RegistrationDialog::userRegistered, this, &MainWindow::onUserRegistered);
        connect(registrationDialog, &RegistrationDialog::switchToLoginDialog, this, &MainWindow::handleSwitchToLogin);
    }
    if (loginDialog && loginDialog->isVisible()) {
        loginDialog->hide();
    }
    registrationDialog->exec();
}

void MainWindow::onUserLoggedIn(int userId, const QString& username, bool isAdmin) {
    isAuthenticated = true;
    currentUserId = userId;
    currentUsername = username;
    isAdminUser = isAdmin;
    
    welcomeLabel->setText("Welcome back, " + currentUsername + "!");
    
    userDashboardAction->setEnabled(true);
    viewCartAction->setEnabled(true);
    loginAction->setEnabled(false);
    registerAction->setEnabled(false);
    logoutAction->setEnabled(true);
    
    adminDashboardAction->setEnabled(isAdmin);
    
    emit userUpdated(userId);
    
    if (isAdmin) {
        QMessageBox::information(this, "Admin Login", 
                               "Welcome, Admin " + currentUsername + "! Opening admin dashboard.");
        QTimer::singleShot(0, this, SLOT(adminDashboard()));
    } else {
        QMessageBox::information(this, "Login Success", 
                               "Welcome, " + currentUsername + "! Showing products.");
        viewProducts();
    }
}

void MainWindow::onUserRegistered(int userId, const QString& username, bool isAdmin) {
    emit userAdded(userId);
    
    QMessageBox::information(this, "Registration Successful", 
                           "Account for " + username + " created. Please login.");
    
    if (registrationDialog && registrationDialog->isVisible()) {
        registrationDialog->accept();
    }
    
    QTimer::singleShot(0, this, SLOT(showLoginDialog())); 
}

void MainWindow::handleSwitchToRegister() {
    if (loginDialog) {
        loginDialog->accept();
    }
    QTimer::singleShot(0, this, SLOT(showRegisterDialog()));
}

void MainWindow::handleSwitchToLogin() {
    if (registrationDialog) {
        registrationDialog->accept();
    }
    QTimer::singleShot(0, this, SLOT(showLoginDialog()));
}

void MainWindow::viewProducts() {
    QFileInfo productsFile("data/products.txt");
    
    if (!productsFile.exists() || productsFile.size() == 0) {
        QDir dataDir;
        if (!dataDir.exists("data")) {
            dataDir.mkdir("data");
        }
        
        std::ofstream outFile("data/products.txt");
        if (outFile.is_open()) {
            outFile << "101,Laptop Pro X,Electronics,1299.99,4.5,10\n";
            outFile << "102,Wireless Mouse,Electronics,24.99,4.2,50\n";
            outFile << "103,Programming in C++,Books,49.99,4.8,25\n";
            outFile << "104,Coffee Maker,Appliances,89.99,4.1,15\n";
            outFile << "105,Smartphone XS,Electronics,899.99,4.7,8\n";
            outFile << "106,Mystery Novel,Books,15.99,3.9,30\n";
            outFile << "107,Bluetooth Speaker,Electronics,79.99,4.4,20\n";
            outFile << "108,Fitness Tracker,Wearables,129.99,4.3,12\n";
            outFile << "109,Kitchen Blender,Appliances,59.99,4.0,18\n";
            outFile << "110,Desk Lamp,Home,34.99,3.8,40\n";
            outFile.close();
            
            QMessageBox::information(this, "Sample Data Created", 
                "Created sample product data for demonstration.");
                
            onProductDataChanged();
        } else {
            QMessageBox::warning(this, "Data Error", 
                "Could not create sample product data. Product listing may be empty.");
        }
    }
    
    if (!productListingWidget) {
        productListingWidget = new ProductListingWidget(this);
        
        connect(productListingWidget, &ProductListingWidget::productSelected, 
                this, &MainWindow::showProductDetails);
        
        connect(this, &MainWindow::productAdded, productListingWidget, &ProductListingWidget::onProductAdded);
        connect(this, &MainWindow::productUpdated, productListingWidget, &ProductListingWidget::onProductUpdated);
        connect(this, &MainWindow::productRemoved, productListingWidget, &ProductListingWidget::onProductRemoved);
    }
    
    setCentralWidget(productListingWidget);
    
    statusLabel->setText("Browsing products");
}

void MainWindow::showProductDetails(int productId) {
    ProductDetailDialog detailDialog(productId, isAuthenticated, currentUserId, this);
    
    connect(&detailDialog, &ProductDetailDialog::addToCartRequested, 
            this, &MainWindow::handleAddToCart);
    connect(&detailDialog, &ProductDetailDialog::addToWishlistRequested, 
            this, &MainWindow::handleAddToWishlist);
    
    connect(&detailDialog, &ProductDetailDialog::reviewAdded,
            [this, productId](int userId) {
                emit reviewAdded(productId, userId);
            });
    
    detailDialog.exec();
}

void MainWindow::handleAddToCart(int productId, int quantity) {
    if (!isAuthenticated) {
        QMessageBox::warning(this, "Authentication Required", 
                           "Please login to add items to your cart.");
        return;
    }
    
    ShoppingCart cart(currentUserId);
    
    if (cart.addToCart(productId, quantity)) {
        QMessageBox::information(this, "Added to Cart", 
                               "Product added to your cart successfully.");
        
        emit cartItemAdded(currentUserId, productId);
    } else {
        QMessageBox::warning(this, "Cart Error", 
                           "Failed to add product to cart. Please try again.");
    }
}

void MainWindow::handleAddToWishlist(int productId) {
    if (!isAuthenticated) {
        QMessageBox::warning(this, "Authentication Required", 
                           "Please login to add items to your wishlist.");
        return;
    }
    
    Wishlist wishlist(currentUserId);
    
    if (wishlist.addToWishlist(productId)) {
        QMessageBox::information(this, "Added to Wishlist", 
                               "Product added to your wishlist successfully.");
        
        emit wishlistItemAdded(currentUserId, productId);
    } else {
        QMessageBox::warning(this, "Wishlist Error", 
                           "Failed to add product to wishlist. Please try again.");
    }
}

void MainWindow::showShoppingCartDialog() {
    if (!isAuthenticated) {
        QMessageBox::warning(this, "Authentication Required", 
                           "Please login to view your cart.");
        return;
    }
    
    ShoppingCartDialog cartDialog(currentUserId, this);
    
    connect(&cartDialog, &ShoppingCartDialog::cartUpdated, 
            this, &MainWindow::onCartDataChanged);
            
    connect(&cartDialog, &ShoppingCartDialog::orderPlaced,
            [this](int orderId) {
                emit orderPlaced(currentUserId, orderId);
            });
    
    cartDialog.exec();
    
    statusLabel->setText("Cart viewed");
}

void MainWindow::showUserDashboardDialog() {
    if (!isAuthenticated) {
        QMessageBox::warning(this, "Authentication Required", 
                           "Please login to view your dashboard.");
        return;
    }
    
    UserDashboardDialog dashboardDialog(currentUserId, currentUsername, this);
    
    connect(&dashboardDialog, &UserDashboardDialog::userProfileUpdated,
            [this]() {
                emit userUpdated(currentUserId);
            });
    
    connect(&dashboardDialog, &UserDashboardDialog::wishlistUpdated,
            this, &MainWindow::onWishlistDataChanged);
    
    connect(&dashboardDialog, &UserDashboardDialog::orderStatusViewed,
            this, &MainWindow::onOrderDataChanged);
    
    dashboardDialog.exec();
    
    statusLabel->setText("Dashboard viewed");
}

void MainWindow::adminDashboard() {
    if (!isAdminUser) {
        QMessageBox::warning(this, "Admin Access Required", 
                           "You need admin privileges to access this feature.");
        return;
    }
    
    AdminDashboardDialog adminDialog(this);
    
    connect(&adminDialog, &AdminDashboardDialog::productAdded,
            [this](int productId) {
                emit productAdded(productId);
            });
    
    connect(&adminDialog, &AdminDashboardDialog::productUpdated,
            [this](int productId) {
                emit productUpdated(productId);
            });
    
    connect(&adminDialog, &AdminDashboardDialog::productRemoved,
            [this](int productId) {
                emit productRemoved(productId);
            });
    
    connect(&adminDialog, &AdminDashboardDialog::userUpdated,
            [this](int userId) {
                emit userUpdated(userId);
            });
    
    connect(&adminDialog, &AdminDashboardDialog::userRemoved,
            [this](int userId) {
                emit userRemoved(userId);
            });
    
    connect(&adminDialog, &AdminDashboardDialog::orderStatusUpdated,
            [this](int orderId, const QString& newStatus) {
                emit orderStatusChanged(orderId, newStatus);
            });
    
    adminDialog.exec();
    
    statusLabel->setText("Admin operations completed");
}


void MainWindow::onProductDataChanged() {
    if (productListingWidget) {
        productListingWidget->refreshProductList();
    }
    
    statusLabel->setText("Product data updated");
    QTimer::singleShot(3000, [this]() {
        statusLabel->setText("Ready");
    });
}

void MainWindow::onCartDataChanged() {
    statusLabel->setText("Cart updated");
    QTimer::singleShot(3000, [this]() {
        statusLabel->setText("Ready");
    });
}

void MainWindow::onOrderDataChanged() {
    statusLabel->setText("Order data updated");
    QTimer::singleShot(3000, [this]() {
        statusLabel->setText("Ready");
    });
}

void MainWindow::onWishlistDataChanged() {
    statusLabel->setText("Wishlist updated");
    QTimer::singleShot(3000, [this]() {
        statusLabel->setText("Ready");
    });
}

void MainWindow::onUserDataChanged() {
    statusLabel->setText("User data updated");
    QTimer::singleShot(3000, [this]() {
        statusLabel->setText("Ready");
    });
}

void MainWindow::onReviewDataChanged() {
    statusLabel->setText("Review data updated");
    QTimer::singleShot(3000, [this]() {
        statusLabel->setText("Ready");
    });
}

void MainWindow::handleLogout() {
    isAuthenticated = false;
    currentUserId = 0;
    currentUsername = "";
    isAdminUser = false;
    
    welcomeLabel->setText("Please login or register to continue.");
    
    loginAction->setEnabled(true);
    registerAction->setEnabled(true);
    viewCartAction->setEnabled(false);
    userDashboardAction->setEnabled(false);
    adminDashboardAction->setEnabled(false);
    logoutAction->setEnabled(false); 
    
    QMessageBox::information(this, "Logged Out", "You have been successfully logged out.");
    
    if (productListingWidget) {
        QWidget* oldWidget = centralWidget();
        
        QWidget *newCentralWidget = new QWidget(this);
        QVBoxLayout *centralLayout = new QVBoxLayout(newCentralWidget);
        centralLayout->addWidget(welcomeLabel);
        centralLayout->addStretch();
        setCentralWidget(newCentralWidget);
        
        if (oldWidget && oldWidget != newCentralWidget) {
            oldWidget->deleteLater();
        }
    }
    
    QTimer::singleShot(100, this, &MainWindow::showLoginDialog);
} 