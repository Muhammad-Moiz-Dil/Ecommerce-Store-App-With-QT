# E-commerce Project Implementation Plan (Revised)

This document outlines the development steps for the C++ E-commerce application based on the project plan and current implementation status.

**Legend:**
*   [X] = Completed
*   [P] = Partially Implemented / Needs Refinement
*   [ ] = Not Started

## Phase 1: Core User and Product Functionality (Completed)

1.  **[X] User Management (`User` Class):**
    *   Implemented user registration (`registerUser`).
    *   Implemented user login (`loginUser`).
    *   Implemented user logout (`logoutUser`).
    *   Data storage in `data/users.txt` (Format: `userID,username,password,email,isAdmin`).
    *   Basic password handling (plain text storage).
    *   Implemented `getNextUserID()` helper.
    *   Uses dynamic `char*` for strings with manual memory management.

2.  **[X] Product Catalog (`Product` Class):**
    *   Implemented product display (`displayProduct`).
    *   Implemented adding products (`addProduct`) - Admin only (static method).
    *   Implemented editing products (`editProduct`) - Admin only (static method).
    *   Implemented removing products (`removeProduct`) - Admin only (static method).
    *   Data storage in `data/products.txt` (Format: `productID,name,category,price,rating,stock`). File rewrite for modifications via temp files.
    *   Implemented `getNextProductID()` helper.
    *   Implemented `loadAllProducts()` helper (returns dynamic array).
    *   Implemented `getProductByID()` helper (public static).
    *   Uses dynamic `char*` for strings with manual memory management.

3.  **[X] Admin Dashboard (`Admin` Class - Inherits `User`):**
    *   Implemented admin check based on `isAdmin` flag (used primarily during login).
    *   Integrated product management methods via wrappers (`adminAddProduct`, `adminEditProduct`, `adminRemoveProduct`) calling static `Product` methods.
    *   Implemented `viewInventory()` and `viewAllUsers()` (reads relevant files).
    *   Placeholders added for `viewOrders()` and `updateOrderStatus()`.

## Phase 2: Shopping and Ordering (Partially Completed)

4.  **[X] Shopping Cart (`ShoppingCart` Class):**
    *   Implemented adding items to cart (`addToCart`) with stock check.
    *   Implemented removing items from cart (`removeFromCart`).
    *   Implemented viewing cart (`viewCart`), fetching product details via `Product::getProductByID()`.
    *   Implemented total calculation (`calculateTotal`).
    *   Implemented `isEmpty()` check.
    *   Implemented `clearCart()`.
    *   Data storage in individual user cart files `data/cart/cart_<userID>.txt` (Format: `productID,quantity`).
    *   Uses dynamic `char*` for filename, file rewrites via temp files for modifications.
    *   Added `getCartFilename()` getter.

5.  **[P] Order Processing (`Order` Class):**
    *   [X] Implemented placing an order (`placeOrder`), moving items from cart to order, using `<ctime>` for date.
    *   [X] Implemented stock reduction in `products.txt` during `placeOrder`.
    *   [X] Implemented appending to `data/orders/orders.txt` (Format: `orderID,userID,productID:qty|...,date,status`).
    *   [X] Implemented `getNextOrderID()` helper.
    *   [X] Implemented static `trackOrder(int orderID)` to display specific order details.
    *   [X] Implemented static `updateStatus(int orderID, const char* newStatus)` for admin use (rewrites `orders.txt`).
    *   [X] Implemented static `viewAllOrders()` for admin use.
    *   [ ] **TODO:** Implement `viewOrdersForUser(int userID)` for customer dashboard.
    *   [P] **TODO:** Refine `placeOrder` in `main.cpp` to ensure `Order` object is correctly initialized with `userID` before calling `placeOrder` method.
    *   [ ] No separate `data/orders/<userID>.txt` files implemented as per original plan (using main `orders.txt` only for now).
    *   Uses dynamic `char*` for strings, file rewrites via temp files for updates.

6.  **[X] Inventory Management:**
    *   Stock reduction implemented within `Order::placeOrder`.
    *   Admin view for inventory implemented via `Admin::viewInventory` (uses `Product::loadAllProducts`).
    *   Manual stock updates via `Admin::adminEditProduct`.
    *   Low stock alerts implemented visually in `Admin::viewInventory`. 

## Phase 3: Enhancements and User Experience (Partially Completed)

7.  **[X] Payment Simulation (`Payment` Class):**
    *   Implemented `simulatePayment()` for various methods (VISA, Mastercard, JazzCash, EasyPaisa, PayPak) with simulated detail prompts.
    *   Includes basic random simulation for success/failure (80% success).
    *   Logs payment attempts in `data/payments.txt` (Format: `paymentID,orderID,userID,amount,method,status`).
    *   Implemented `getNextPaymentID()` helper.

8.  **[X] Reviews and Ratings (`Review` Class):**
    *   Implemented adding reviews (`addReview`), checking for duplicates per user/product.
    *   Implemented fetching/displaying reviews for a product (`getReviewsForProduct`).
    *   Stores reviews in `data/reviews/reviews.txt` (Format: `productID,userID,rating,comment`). Commas in comments replaced with semicolons.
    *   Implemented automatic update of product average rating in `products.txt` after adding a review (using `updateProductAverageRating` helper).
    *   [ ] No separate `data/reviews/<productID>.txt` files implemented.
    *   Constraint compliance fixed (removed `<vector>`, `<numeric>`).

9.  **[X] Wishlist Management (`Wishlist` Class):**
    *   Implemented adding to wishlist (`addToWishlist`), checking for duplicates.
    *   Implemented removing from wishlist (`removeFromWishlist`).
    *   Implemented viewing wishlist (`viewWishlist`), fetching product details.
    *   Data storage in `data/wishlist/wishlist_<userID>.txt` (Format: `productID`).
    *   Uses dynamic `char*` for filename, file rewrites via temp files.

10. **[X] Search and Filtering:**
    *   Implemented search by product name (case-insensitive partial match) via `Product::searchByName`.
    *   Implemented filtering by category (case-insensitive exact match) via `Product::filterByCategory`.
    *   Implemented filtering by price range via `Product::filterByPriceRange`.
    *   Implemented filtering by minimum rating via `Product::filterByRating`.
    *   Logic reads `products.txt` and displays filtered results using dynamic arrays and a display helper.

## Phase 4: Finalization (Partially Completed)

11. **[P] Main Application Logic (`main.cpp`):**
    *   [X] Basic main menu and user interaction flow structured.
    *   [X] Integration of Login, Register, Customer Dashboard, Admin Dashboard.
    *   [X] Basic integration of Cart, Wishlist, Product Browsing, Payment Simulation, Order Placement, Admin functions.
    *   [P] **TODO:** Refactor User-to-Admin object handling after successful login (currently uses unsafe `static_cast` as placeholder).
    *   [X] Handles basic user input and navigation.

12. **[ ] Testing and Debugging:**
    *   Needs thorough testing of all features.
    *   Track bugs in `docs/bug_tracking.md`.

13. **[P] Documentation:**
    *   [X] Initial documentation files created (`implementation_plan.md`, `project_structure.md`, `ux_design.md`, `bug_tracking.md`).
    *   [P] Documentation updated to reflect current status (this revision).
    *   [ ] Needs `README.md` with setup/compilation instructions.

## Constraints Reminder (Status: Mostly Adhered)

*   C++, OOP, Header-only (`.h` files). [CHECK - Adhered] 
*   Single `main.cpp`. [CHECK - Adhered]
*   Restricted libraries: `<iostream>`, `<cstring>`, `<ctime>`, `<fstream>`, `<cmath>`, `<iomanip>`, `<sstream>`, `<string>`, `<limits>`, `<cstdio>`. [CHECK - Adhered, removed vector/numeric]
*   `using namespace std;`. [CHECK - Adhered]
*   Only `strlen`, `strcpy`, `.length()` allowed from standard libraries beyond basics. [CHECK - Adhered]
*   No structs (within class definitions). [CHECK - Adhered]
*   No `make`. Compile with `g++`. [CHECK - Assumed, needs adding to README]
*   Console error messages only. [CHECK - Adhered]
*   Data in `data/*.txt`. File rewrites for updates. [CHECK - Adhered]
*   Dynamic memory for all arrays / relevant strings. No memory leaks (requires careful checking). [CHECK - Implemented, requires testing]
*   Keep complexity suitable for a 2nd-semester level. [CHECK - Adhered] 