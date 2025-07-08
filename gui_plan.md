# GUI Implementation Plan for E-commerce Project using Qt

This document outlines the plan for developing a Qt-based Graphical User Interface (GUI) for the C++ E-commerce project.

## 1. Project Setup with Qt
*   Manually set up a Qt project. This involves preparing for direct compilation using `g++`. You will need to manually identify and specify all source files, necessary Qt module dependencies (e.g., Core, GUI, Widgets), their include paths (using `-I` flags with `g++`), and the corresponding libraries to link against (using `-L` and `-l` flags with `g++`).
*   Integrate the existing header-only C++ backend logic (`src/*.h` files) by ensuring the `src` directory is added to the include paths specified with the `-I` flag in your `g++` command.
*   Ensure the project can be compiled using a direct `g++` command. This command must include all C++ source files (both project-specific and Qt-generated moc files, if any), the necessary include paths for your project's `src` directory and Qt modules, library paths for Qt, and links against all required Qt libraries as well as your existing C++ backend code.
*   The `main.cpp` from the original console-based project will be replaced or heavily modified by a new `main.cpp` that initializes the `QApplication` and shows the main Qt window.

## 2. Core Application Window
*   Design a main window (`QMainWindow`) that will serve as the primary interface.
*   Implement a menu bar and/or toolbar for common actions (e.g., Login, Register, View Products, View Cart, Exit).

## 3. User Authentication Screens
*   **Login Window/Dialog (`QDialog`):**
    *   Fields for username and password.
    *   "Login" and "Register" buttons.
    *   Connect to the existing `User::loginUser()` backend function.
    *   Display error messages for invalid credentials using `QMessageBox`.
*   **Registration Window/Dialog (`QDialog`):**
    *   Fields for username, password, email.
    *   "Register" button.
    *   Connect to the existing `User::registerUser()` backend function.
    *   Display success/error messages using `QMessageBox`.

## 4. Product Display and Interaction
*   **Product Listing View (`QWidget` or `QListWidget`/`QTableWidget`):**
    *   Display products fetched using `Product::loadAllProductsAndDisplay()`.
    *   Show key details: name, price, category, rating.
    *   Implement search and filter functionality (name, category, price range, rating) using existing `Product` class static methods (e.g., `Product::searchByName()`, `Product::filterByCategory()`). The UI will trigger these backend methods and update the view.
    *   Allow users to click on a product to view details.
*   **Product Detail View (`QWidget` or `QDialog`):**
    *   Show comprehensive product information (using `Product::getProductByID()`).
    *   "Add to Cart" button (connects to `ShoppingCart::addToCart()`).
    *   "Add to Wishlist" button (connects to `Wishlist::addToWishlist()`).
    *   Display product reviews (using `Review::getReviewsForProduct()`).
    *   Allow users to add a review (UI for `Review::addReview()`). This will also require identifying the ordered products, possibly using `Order::getProductIDsForOrder()` if the review is prompted post-purchase.

## 5. Shopping Cart Management
*   **Shopping Cart View (`QWidget` or `QDialog`):**
    *   Display items in the cart using `ShoppingCart::viewCart()`.
    *   Show item details, quantity, and price.
    *   Allow users to remove items (`ShoppingCart::removeFromCart()`).
    *   Display total cart value (`ShoppingCart::calculateTotal()`).
    *   "Proceed to Checkout" button.

## 6. Checkout Process
*   **Order Summary/Confirmation (`QWidget` or `QDialog`):**
    *   Display order details before finalization.
*   **Payment Simulation (`QWidget` or `QDialog`):**
    *   Interface for `Payment::simulatePayment()`.
    *   Allow selection of payment method.
    *   On successful payment simulation, proceed to place the order.
*   Connect to `Order::placeOrder()` after successful payment (requires a `ShoppingCart` object).
*   Update UI to reflect order status and provide confirmation via `QMessageBox`.

## 7. User Dashboard (Customer)
*   A dedicated section or tabbed widget for logged-in users.
*   **Profile View/Edit (`QWidget`):**
    *   Display current user information (username, email).
    *   Allow users to update their email and password.
    *   **Backend Requirement:** Requires new methods in `User.h`, such as `bool User::updateEmail(const char* newEmail)` and `bool User::updatePassword(const char* currentPassword, const char* newPassword)`.
*   **Order History View (`QWidget` or `QTableWidget`):**
    *   Display user's past orders (using `Order::viewOrdersForUser(int userID)`).
    *   Allow tracking order status by calling `Order::trackOrder(int orderID)`. (Consider verifying order ownership in backend if `trackOrder` is made more secure).
*   **Wishlist View (`QWidget` or `QListWidget`):**
    *   Display items in the wishlist using `Wishlist::viewWishlist()`.
    *   Allow removing items from the wishlist (UI for `Wishlist::removeFromWishlist()`).
    *   (Optional Feature: UI to move item from wishlist to cart, using `ShoppingCart::addToCart()` and `Wishlist::removeFromWishlist()`).

## 8. Admin Dashboard
*   A separate section, window, or set of dialogs accessible after an admin login. Assumes an `Admin` object is created/accessible post-login for calling its member functions.
*   **Product Management (`QWidget` with `QTableWidget`):**
    *   View all products (e.g., using `Product::loadAllProductsAndDisplay()` or as part of `Admin::viewInventory()`).
    *   UI to trigger `adminObject.adminAddProduct()` (member function of `Admin` class) with input fields for product details.
    *   UI to trigger `adminObject.adminEditProduct()` by selecting a product and modifying its details.
    *   UI to trigger `adminObject.adminRemoveProduct()` by selecting a product.
*   **User Management (`QWidget` with `QTableWidget`):**
    *   View all users (using `Admin::viewAllUsers()` - static method in `Admin.h`, or `User::viewAllUsers()`).
    *   UI to trigger `User::editUser()` (static method) by selecting a user and modifying details.
    *   UI to trigger `User::removeUser(adminUserID)` (static method) by selecting a user.
    *   (Consideration: Add wrapper methods like `adminObject.adminEditUser()` and `adminObject.adminRemoveUser()` in `Admin.h` for consistency, which would then call the static `User` methods.)
*   **Order Management (`QWidget` with `QTableWidget`):**
    *   View all orders (UI calls `Admin::viewOrders()` if implemented, otherwise `Order::viewAllOrders()` - static).
    *   UI to update order status (UI calls `Admin::updateOrderStatus()` if implemented, otherwise `Order::updateStatus()` - static).
    *   (Note: `Admin.h` has placeholder methods `viewOrders()` and `updateOrderStatus()`. If these are implemented, they should ideally call the static `Order` methods or contain the full logic.)
*   **Inventory View (`QWidget`):**
    *   Display product inventory details using `adminObject.viewInventory()` (member function of `Admin` class).

## 9. Data Handling and UI Updates
*   Use Qt's signals and slots mechanism for communication between UI elements (e.g., button clicks) and backend C++ class methods (either static methods or member functions of instantiated objects like `Admin`, `ShoppingCart`, `Wishlist`).
*   When backend data changes (e.g., product added, order placed, cart updated), the relevant UI views should refresh. This typically involves:
    *   The UI action calls the backend C++ method.
    *   The backend method updates the `.txt` data file.
    *   The UI re-fetches the necessary data from the backend (which reads the updated file) and updates the Qt model/view components (e.g., repopulating a `QTableWidget`).
*   Since the backend relies on text file I/O, ensure UI updates are efficient. For a 2nd-semester project, direct synchronous file operations triggered by UI events are acceptable.

### Implementation Status - COMPLETED ✅

The core data handling and UI update functionality has been implemented successfully, with proper signals/slots communication between UI components and backend classes.

#### Completed Components:

- ✅ **ProductListingWidget**: Fully implemented with:
  - Loading products from `data/products.txt` via `Product::loadAllProducts()`
  - Search/filter functionality with UI updates
  - Signal emission when products are selected
  - Refresh methods (`refreshProductList()`, `onProductAdded()`, etc.)

- ✅ **ProductDetailDialog**: Fully implemented with:
  - Product details loading via `Product::getProductByID()`
  - "Add to Cart" button connects to `ShoppingCart::addToCart()`
  - "Add to Wishlist" button connects to `Wishlist::addToWishlist()`
  - Tabbed interface with product details and reviews
  - Review functionality properly integrated (see Review section below)

- ✅ **ShoppingCartDialog**: Implemented with:
  - Loading cart items with `ShoppingCart::viewCart()`
  - UI for removing items with `ShoppingCart::removeFromCart()`
  - Quantity updates and checkout functionality
  - Total calculation with `ShoppingCart::calculateTotal()`

- ✅ **OrderSummaryDialog** and **PaymentDialog**: Implemented with:
  - Order summary display before checkout
  - Payment method selection
  - Payment simulation
  - Order placement functionality (though partially simulated)

- ✅ **WishlistWidget**: Implemented with:
  - Wishlist display via `Wishlist::viewWishlist()`
  - Removal functionality with `Wishlist::removeFromWishlist()`
  - Option to move items to cart (combining `ShoppingCart::addToCart()` and `Wishlist::removeFromWishlist()`)

- ✅ **UserDashboardDialog**: Implemented with tabs for:
  - User profile information (UserProfileWidget)
  - Order history (OrderHistoryWidget)
  - Wishlist items (WishlistWidget)
  - Cross-component data synchronization with signals/slots

- ✅ **AdminDashboardDialog**: Implemented with tabs for:
  - Product management (ProductManagementWidget)
  - User management (UserManagementWidget)
  - Order management (OrderManagementWidget)
  - Inventory view (InventoryViewWidget)

- ✅ **ReviewWidget**: Fully implemented with:
  - Display of existing product reviews using data from `data/reviews/reviews.txt`
  - Form for users to add new reviews with ratings (1-5 stars) and comments
  - Integration with `Review::addReview()` backend method
  - Purchase verification to ensure users can only review products they've bought
  - User identification for reviews using `User::getUserEmailById()` 
  - Real-time UI updates when new reviews are added

- ✅ **Data Flow Pattern**: Successfully implemented where:
  1. UI actions trigger backend methods
  2. Backend operations update data files
  3. UI refreshes to show updated data

#### Specific Implementation Details:

1. **File-based Data Storage**:
   - All components correctly read from and write to the appropriate `.txt` files in the `data/` directory
   - User data is stored in `data/users.txt`
   - Product data is stored in `data/products.txt` 
   - Cart items in `data/cart/cart_[userID].txt`
   - Wishlist items in `data/wishlist/wishlist_[userID].txt`
   - Orders in `data/orders/`
   - Reviews in `data/reviews/reviews.txt`

2. **Cross-component Communication**:
   - MainWindow acts as the central hub for component communication
   - Components emit signals that are caught by MainWindow slots
   - MainWindow refreshes appropriate views after data changes

3. **Refresh Pattern**:
   - After UI actions that modify backend data, views are refreshed to show updated data
   - The `refreshViews()` method in MainWindow ensures the ProductListingWidget is updated
   - Each component has its own refresh method (e.g., `refreshProductList()`, `refreshWishlist()`, `refreshReviews()`)

### Features Missing or Partially Implemented from Step 9

All features of Step 9 have now been fully implemented:

- ✅ **Admin Functionality Limitations** - RESOLVED:
  - The admin dashboard UI components are now fully implemented and connected to backend functionality:
    - ✅ OrderManagementWidget now uses real Order class methods to load and update orders
    - ✅ InventoryViewWidget uses Product::loadAllProducts instead of placeholder data
    - ✅ ProductManagementWidget uses real Product class methods for all product operations
    - ✅ UserManagementWidget now uses real User class methods for loading, editing and removing users
  - Some admin functionality still has limitations:
    - ⚠️ Admin adding users directly is not implemented (self-registration is preferred)

- ✅ **OrderSummaryDialog and PaymentDialog Integration** - RESOLVED:
  - The OrderSummaryDialog and PaymentDialog have been fully integrated with the backend:
    - ✅ OrderSummaryDialog now calls Order::placeOrder with the appropriate ShoppingCart object
    - ✅ PaymentDialog has been enhanced with detailed payment method interfaces
    - ✅ The complete order flow works: cart → order summary → payment → order placement

- ✅ **Data Synchronization Improvements** - RESOLVED:
  - Implemented a robust signal-slot system for selective data updates:
    - ✅ Added specific signals in MainWindow for all types of data changes (products, cart, orders, etc.)
    - ✅ Components now listen for specific changes rather than doing full refreshes
    - ✅ UI updates are now targeted to only the affected data
    - ✅ Status bar updates provide user feedback on data operations

## 10. Error Handling and User Feedback
*   Use `QMessageBox` extensively for:
    *   Displaying errors from backend operations (e.g., login failed, product out of stock).
    *   Confirming actions (e.g., "Are you sure you want to remove this item?").
    *   Providing informational messages (e.g., "Order placed successfully!").

## 11. Styling and UX
*   Utilize Qt's styling capabilities (e.g., QSS - Qt Style Sheets) to apply a basic, clean, and consistent look and feel.
*   Refer to `docs/ux_design.md` for any specific UI/UX guidelines if available and applicable to a Qt GUI.


