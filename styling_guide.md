# Dark Mode UI Styling Guide

This document provides guidelines on how to use the dark mode styling for the E-commerce Qt application.

## Overview

The application uses a Qt Style Sheet (QSS) for consistent dark mode styling across all UI components. The styles are defined in `styles.qss` and loaded at application startup.

## How to Apply Styles to Components

### Using Object Names

You can target specific widgets using their object names:

```cpp
QPushButton* addButton = new QPushButton("Add to Cart");
addButton->setObjectName("add-to-cart-button");
```

Then in the QSS:

```css
QPushButton#add-to-cart-button {
    background-color: #43A047;
    /* other styles */
}
```

### Using Classes

You can apply styles to groups of widgets by setting a "class" property:

```cpp
QPushButton* deleteButton = new QPushButton("Delete");
deleteButton->setProperty("class", "danger");
```

Then in the QSS:

```css
QPushButton.danger {
    background-color: #E53935;
    /* other styles */
}
```

### Styling for Specific Components

#### Headings and Text

```cpp
// For a main heading
QLabel* titleLabel = new QLabel("Product Details");
titleLabel->setObjectName("heading");

// For a subheading
QLabel* categoryLabel = new QLabel("Category");
categoryLabel->setObjectName("subheading");
```

#### Buttons

```cpp
// Primary action button (default style)
QPushButton* saveButton = new QPushButton("Save");

// Secondary action button
QPushButton* cancelButton = new QPushButton("Cancel");
cancelButton->setProperty("class", "secondary");

// Success action button
QPushButton* addToCartButton = new QPushButton("Add to Cart");
addToCartButton->setProperty("class", "success");

// Danger action button
QPushButton* deleteButton = new QPushButton("Delete");
deleteButton->setProperty("class", "danger");
```

#### Status Labels

```cpp
// For order status indicators
QLabel* statusLabel = new QLabel("Pending");
statusLabel->setProperty("class", "status-pending");

// Other status classes:
// - status-processing
// - status-shipped
// - status-delivered
// - status-cancelled
```

## Color Palette Reference

### Primary Colors
- Primary Blue (`#2979FF`): Main brand color
- Primary Light (`#5393FF`): Hover states
- Primary Dark (`#0D47A1`): Active states

### Secondary Colors
- Accent Green (`#43A047`): Success indicators
- Accent Red (`#E53935`): Error states, warnings
- Accent Orange (`#FB8C00`): Notifications, warnings

### Neutral Colors
- Dark Background (`#121212`): Main background
- Surface Dark (`#1E1E1E`): Cards, dialogs
- Dark Gray (`#2D2D2D`): Secondary surfaces
- Medium Gray (`#757575`): Borders, dividers
- Light Gray (`#BBBBBB`): Secondary text
- White (`#FFFFFF`): Primary text

## Custom Widgets

### Product List Items

When creating custom widgets for product listings, add appropriate classes to enable styling:

```cpp
// In your ProductListItem class constructor:
this->setProperty("class", "ProductListItem");

// For child elements:
nameLabel->setProperty("class", "title");
priceLabel->setProperty("class", "price");
ratingLabel->setProperty("class", "rating");
```

### Shopping Cart Items

```cpp
// In your CartItem class constructor:
this->setProperty("class", "CartItem");

// For child elements:
quantityLabel->setProperty("class", "quantity");
removeButton->setProperty("class", "remove-button");
```

## Tips for Custom Dialogs

1. Use the existing styling as much as possible for consistency
2. For dialog titles:
   ```cpp
   QLabel* titleLabel = new QLabel("Dialog Title");
   titleLabel->setObjectName("dialogTitle");
   ```
3. Group related inputs using QGroupBox which has predefined styling

## Troubleshooting

If styles aren't applying correctly:

1. Check that the object name or class property is set correctly
2. Verify that the styles.qss file is loading properly
3. For complex widgets, inspect the widget hierarchy to ensure styles are targeting the correct elements

## Extending the Stylesheet

If you need to add new styles:

1. Add them to the existing `styles.qss` file following the established patterns
2. Follow the naming conventions used in the current stylesheet
3. Keep the dark mode color palette consistent 