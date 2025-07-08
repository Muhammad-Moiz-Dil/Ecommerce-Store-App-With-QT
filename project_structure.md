# Project Structure

This document outlines the directory and file structure of the C++ E-commerce project.

```
ECOM_APP/
├── src/                      # Source files (Header-only implementations)
│   ├── User.h
│   ├── Product.h
│   ├── Admin.h               # (Inherits User)
│   ├── ShoppingCart.h
│   ├── Order.h
│   ├── Payment.h
│   ├── Review.h
│   ├── Wishlist.h
│   └── utility.h             # (Optional: For shared helper functions if needed)
├── data/                     # Data storage files
│   ├── users.txt
│   ├── products.txt
│   ├── payments.txt
│   ├── cart/                 # Directory for individual cart files
│   │   └── cart_<userID>.txt
│   ├── orders/               # Directory for order-related files
│   │   ├── orders.txt        # Main orders log
│   │   └── <userID>.txt      # Individual user order details
│   ├── reviews/              # Directory for review-related files
│   │   ├── reviews.txt       # Main reviews log
│   │   └── <productID>.txt   # (Optional: Reviews per product)
│   ├── wishlist/             # Directory for wishlist files
│   │   └── <userID>.txt
├── docs/                     # Documentation files
│   ├── implementation_plan.md
│   ├── bug_tracking.md
│   ├── project_structure.md
│   └── ux_design.md
├── main.cpp                  # Main application entry point
└── README.md                 # Project overview and setup instructions
```

## Key Components:

*   **`src/`**: Contains all C++ header files (`.h`) with class definitions and implementations, adhering to the header-only constraint.
*   **`data/`**: Holds all `.txt` files used for persistent data storage. Subdirectories organize user-specific or feature-specific data.
*   **`docs/`**: Contains project documentation.
*   **`main.cpp`**: The single `.cpp` file containing the `main` function and driving the application flow.
*   **`README.md`**: Provides essential information about the project.

**Note:** The exact filenames within `data/cart/`, `data/orders/`, `data/reviews/`, and `data/wishlist/` will be generated dynamically based on user IDs or product IDs. 