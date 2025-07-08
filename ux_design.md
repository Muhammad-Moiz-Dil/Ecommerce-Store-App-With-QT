# User Experience (UX) Design

This document describes the intended user experience and interaction flow for both the console-based and GUI-based versions of the C++ E-commerce application.

## Console Interface (Original Implementation)

### Core Principles

*   **Simplicity:** Menus and prompts should be clear and easy to understand.
*   **Text-Based:** All interactions occur via text input and output in the console.
*   **Menu-Driven:** Navigation relies on numbered menus to guide users through options.
*   **Consistency:** Similar actions (e.g., viewing lists, selecting items) should have a consistent interaction pattern.

### User Flow

1.  **Initial Screen (`main` loop):**
    *   Display a welcome message.
    *   Present options:
        *   `1. Login`
        *   `2. Register`
        *   `3. Exit`

2.  **Registration (`handleRegistration`):**
    *   Calls `User::registerUser()`.
    *   Prompts for `username`, `password`, `email`.
    *   Checks if username exists.
    *   Appends new user (non-admin) to `data/users.txt`.
    *   Provides feedback on success or failure.
    *   Returns to the initial screen.

3.  **Login (`handleLogin`):**
    *   Calls `User::loginUser()`.
    *   Prompts for `username` and `password`.
    *   Loads user data into the `User` object if credentials match `data/users.txt`.
    *   On successful login, `main` loop checks `isAdmin` flag.
    *   Proceeds to `showCustomerDashboard` or `showAdminDashboard`.
    *   On failure, shows an error message and returns to the initial screen.
    *   **Note:** Transition from `User` object to `Admin` context needs refactoring for safety.

4.  **Customer Dashboard (`showCustomerDashboard`):**
    *   Displays welcome message with username/ID.
    *   Creates `ShoppingCart` and `Wishlist` objects for the user.
    *   Presents options:
        *   `1. View Products (Browse/Search/Filter)`
        *   `2. View Shopping Cart`
        *   `3. View Wishlist`
        *   `4. View Order History`
        *   `5. Logout`

5.  **Viewing Products (`handleProductBrowsing`):**
    *   Initially displays all products (`Product::loadAllProductsAndDisplay`).
    *   Offers sub-menu for actions:
        *   `1. Search by Name` (`Product::searchByName`)
        *   `2. Filter by Category` (`Product::filterByCategory`)
        *   `3. Filter by Price Range` (`Product::filterByPriceRange`)
        *   `4. Filter by Minimum Rating` (`Product::filterByRating`)
        *   `5. View Product Details by ID` (`Product::getProductByID`)
        *   `6. Add Product to Cart` (Prompts for ID, Qty; calls `ShoppingCart::addToCart`)
        *   `7. Add Product to Wishlist` (Prompts for ID; calls `Wishlist::addToWishlist`)
        *   `8. View Reviews for Product` (Prompts for ID; calls `Review::getReviewsForProduct`)
        *   `9. Show All Products` (`Product::loadAllProductsAndDisplay`)
        *   `0. Back to Customer Dashboard`

6.  **Viewing Cart (`showCustomerDashboard` -> case 2):**
    *   Calls `ShoppingCart::viewCart()` to display items, quantities, prices, and total.
    *   If cart is not empty, offers sub-menu:
        *   `1. Remove Item` (Prompts for ID; calls `ShoppingCart::removeFromCart`)
        *   `2. Proceed to Checkout` (Initiates checkout process)
        *   `0. Back to Menu`

7.  **Checkout Process (`showCustomerDashboard` -> case 2 -> option 2):**
    *   Calculates and displays total (`ShoppingCart::calculateTotal`).
    *   Asks for confirmation (y/n).
    *   If confirmed:
        *   Calls `Payment::simulatePayment()` which handles method selection, simulated detail entry, success/failure simulation, and logging to `data/payments.txt`.
        *   If payment simulation successful:
            *   Creates a temporary `Order` object (initialized with UserID).
            *   Calls `orderToPlace.placeOrder(cart)` which handles reading cart, updating stock, writing to `orders.txt`, and clearing the cart.
            *   Displays success/failure message for order placement.
        *   If payment simulation failed, displays failure message.
    *   Returns to customer dashboard menu.
    *   **Note:** Requires `Order` object to be correctly initialized with UserID.

8.  **Viewing Wishlist (`showCustomerDashboard` -> case 3):**
    *   Calls `Wishlist::viewWishlist()` to display items with details.
    *   If wishlist is not empty, offers sub-menu:
        *   `1. Remove Item` (Prompts for ID; calls `Wishlist::removeFromWishlist`)
        *   `2. Add Item to Cart` (Prompts for ID, Qty; calls `ShoppingCart::addToCart`)
        *   `0. Back to Menu`

9.  **Viewing Order History (`showCustomerDashboard` -> case 4):**
    *   Displays placeholder message.
    *   Currently calls `Order::viewAllOrders()` (shows all orders).
    *   **TODO:** Needs implementation of `Order::viewOrdersForUser(userID)` to show only the current user's orders.

10. **Admin Dashboard (`showAdminDashboard`):**
    *   Displays welcome message for admin.
    *   Presents options:
        *   `1. Manage Products (Add/Edit/Remove)`
        *   `2. View All Orders` (`Order::viewAllOrders`)
        *   `3. Update Order Status` (Prompts for ID, Status; calls `Order::updateStatus`)
        *   `4. View Inventory / Stock Levels` (`Admin::viewInventory`)
        *   `5. View All Users` (`Admin::viewAllUsers`)
        *   `6. Logout`

11. **Product Management (Admin) (`showAdminDashboard` -> case 1):**
    *   Provides sub-menu:
        *   `1. Add Product` (Calls `Admin::adminAddProduct` -> `Product::addProduct`)
        *   `2. Edit Product` (Calls `Admin::adminEditProduct` -> `Product::editProduct`)
        *   `3. Remove Product` (Calls `Admin::adminRemoveProduct` -> `Product::removeProduct`)
        *   `0. Back to Admin Menu`

12. **Order Management (Admin):**
    *   View All Orders handled by `Order::viewAllOrders` (Case 2).
    *   Update Status handled by `Order::updateStatus` (Case 3).

13. **Inventory Management (Admin):**
    *   Handled by `Admin::viewInventory` (Case 4).
    *   Stock editing occurs via Product Edit (Case 1 -> option 2).

### Error Handling

*   Basic messages provided for invalid menu input, file errors, failed operations (e.g., login failed, product not found, insufficient stock, payment failed).
*   Input loops generally handle non-numeric input for menu choices.
*   More robust validation (e.g., for string inputs, specific formats) is minimal.
*   Guides user back to the relevant menu after most errors.

## GUI Interface (Qt Implementation)

### Core Principles

*   **Graphical Interaction:** Users interact with visual elements rather than text-based menus.
*   **Intuitive Navigation:** Menu bars, buttons, and dialog boxes provide clear pathways for actions.
*   **Responsive Feedback:** Visual cues and message boxes inform users about operation results.
*   **Modern Aesthetics:** Clean, consistent visual design throughout the application.
*   **Component-Based Structure:** Separate dialogs and widgets for different functionality.

### User Flow

1. **Initial Screen (MainWindow):**
   * Displays a welcoming message, "Please login or register to continue."
   * Menu bar with options:
     * File: Login, Register, View Products, Exit
     * User: View Cart, User Dashboard, Logout (disabled until login)
     * Admin: Admin Dashboard (disabled until admin login)

2. **Login (LoginDialog):**
   * Modal dialog with fields for username and password.
   * "Login" button to submit credentials.
   * "Register Instead" link to switch to registration dialog.
   * Displays error messages for invalid credentials.
   * On success, updates MainWindow to show welcome message with username.

3. **Registration (RegistrationDialog):**
   * Modal dialog with fields for username, password, email.
   * "Register" button to submit registration.
   * "Login Instead" link to switch to login dialog.
   * Displays success/error messages.
   * On success, automatically switches to login dialog.

4. **Product Browsing (ProductListingWidget):**
   * Displays products in a scrollable list/grid view.
   * Search bar for finding products by name.
   * Filter options for category, price range, and rating.
   * Clicking on a product opens its detailed view.
   * For authenticated users, shows buttons for cart/wishlist actions.

5. **Product Details (ProductDetailDialog):**
   * Modal dialog showing comprehensive product information.
   * Tabbed interface with:
     * Product details: name, price, category, rating, stock
     * Reviews: list of user reviews with ratings
   * "Add to Cart" button with quantity selector.
   * "Add to Wishlist" button.
   * For authenticated users, ability to add reviews.

6. **Shopping Cart (ShoppingCartDialog):**
   * Displays items in cart with details, quantities, and prices.
   * Options to remove items or change quantities.
   * Displays total cart value.
   * "Proceed to Checkout" button.
   * Requires authentication to access.

7. **Checkout Process:**
   * **Order Summary Dialog:**
     * Displays items, quantities, prices, and total.
     * Option to continue or cancel.
   * **Payment Dialog:**
     * Selection of payment method.
     * Simulated payment process.
     * Confirmation on successful payment.
   * On completion, returns to product browsing with confirmation message.

8. **User Dashboard (UserDashboardDialog):**
   * Tabbed interface with:
     * **Profile (UserProfileWidget):** View/edit user information.
     * **Order History (OrderHistoryWidget):** List of past orders with status.
     * **Wishlist (WishlistWidget):** Saved items with options to move to cart or remove.
   * Requires authentication to access.

9. **Admin Dashboard (AdminDashboardDialog):**
   * Tabbed interface with:
     * **Product Management (ProductManagementWidget):** Add, edit, remove products.
     * **User Management (UserManagementWidget):** View, edit, remove users.
     * **Order Management (OrderManagementWidget):** View all orders, update status.
     * **Inventory View (InventoryViewWidget):** Stock levels for all products.
   * Only accessible to admin users.

10. **Logout Process:**
    * Clears current user session.
    * Returns to welcome screen.
    * Re-enables login/register options.
    * Disables user-specific actions.
    * Shows confirmation message.
    * Automatically opens login dialog for new session.

### Error Handling and User Feedback

* **QMessageBox Dialogs:** Used throughout the application for:
  * Error messages for invalid inputs or failed operations.
  * Warning messages requiring user attention.
  * Information messages confirming successful actions.
  * Question dialogs for actions requiring confirmation.

* **Status Bar Updates:** Provides contextual information about:
  * Current application state.
  * Results of recent operations.
  * Time-limited status messages that automatically clear.

* **Visual Indicators:**
  * Enabled/disabled buttons based on current state.
  * Clear labeling of required fields.
  * Confirmation dialogs for destructive actions.

### Data Synchronization

* Changes in one component trigger updates in related components through Qt's signal-slot mechanism.
* Backend operations (file I/O) are performed when needed, with UI refreshing to show current data.
* Specific signals for different types of data changes (products, cart, orders, etc.) ensure targeted updates. 