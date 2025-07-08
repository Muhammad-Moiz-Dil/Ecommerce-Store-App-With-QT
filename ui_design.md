# UI Design and Styling for E-commerce Application (Dark Mode)

This document outlines the styling guidelines and design specifications for the Qt-based GUI of the C++ E-commerce application, implementing the UX requirements specified in `ux_design.md`. The application uses an exclusively dark mode interface.

## Color Palette

### Primary Colors
- **Primary Blue (`#2979FF`)**: Main brand color, used for primary buttons, highlighted elements, and key UI components
- **Primary Light (`#5393FF`)**: Lighter shade for hover states and selected items
- **Primary Dark (`#0D47A1`)**: Darker shade for active states

### Secondary Colors
- **Accent Green (`#43A047`)**: Success indicators, "Add to Cart" buttons, confirmation messages
- **Accent Red (`#E53935`)**: Error states, deletion actions, warnings
- **Accent Orange (`#FB8C00`)**: Notifications, warnings, special offers

### Neutral Colors
- **Dark Background (`#121212`)**: Main background color for the application
- **Surface Dark (`#1E1E1E`)**: Background for cards, dialogs, and raised surfaces
- **Dark Gray (`#2D2D2D`)**: Secondary surfaces, hover states, alternating rows
- **Medium Gray (`#757575`)**: Borders, dividers, disabled elements
- **Light Gray (`#BBBBBB`)**: Secondary text, subtle elements
- **White (`#FFFFFF`)**: Primary text, icons, emphasized elements

## Typography

### Fonts
- **Primary Font**: System default sans-serif font (for cross-platform compatibility)
- **Font Sizes**:
  - Heading Large: 18px
  - Heading Medium: 16px
  - Heading Small: 14px
  - Body Text: 12px
  - Small Text: 10px

### Text Styling
- Headings: Bold weight, white or primary blue color
- Body Text: Regular weight, light gray or white color
- Links: Primary blue color, no underline (underline on hover)
- Emphasis: Bold or primary blue color as appropriate

## Component Styling

### Buttons
- **Primary Button**:
  - Background: Primary Blue
  - Text: White
  - Border: None
  - Border-radius: 4px
  - Padding: 8px 16px
  - Hover: Primary Light
  - Active/Pressed: Primary Dark
  
- **Secondary Button**:
  - Background: Dark Gray
  - Text: White
  - Border: 1px solid Primary Blue
  - Border-radius: 4px
  - Padding: 8px 16px
  - Hover: Primary Blue with 20% opacity
  
- **Success Button** (Add to Cart, Checkout):
  - Background: Accent Green
  - Text: White
  - Border: None
  - Border-radius: 4px
  
- **Danger Button** (Delete, Remove):
  - Background: Accent Red
  - Text: White
  - Border: None
  - Border-radius: 4px

### Input Fields
- Background: Dark Gray
- Border: 1px solid Medium Gray
- Border-radius: 4px
- Text: White
- Padding: 8px
- Focus: Border color changes to Primary Blue
- Invalid: Border color changes to Accent Red

### Dialogs
- Background: Surface Dark
- Border: 1px solid Dark Gray
- Border-radius: 8px
- Box-shadow: 0 4px 20px rgba(0,0,0,0.5)
- Title bar: Primary Blue with white text

### Lists and Tables
- Background: Dark Background
- Alternating row colors: Surface Dark and Dark Gray
- Header: Bold, Medium Gray background
- Selected row: Primary Blue background with 30% opacity
- Hover row: Dark Gray with slight brightness increase
- Borders: 1px solid Dark Gray
- Text: White for active, Light Gray for secondary
- Border-collapse: Collapse for tables

### Cards (Product items)
- Background: Surface Dark
- Border: 1px solid Dark Gray
- Border-radius: 4px
- Box-shadow: 0 2px 8px rgba(0,0,0,0.3)
- Padding: 12px
- Hover: Subtle box-shadow increase and border color highlight

### Navigation
- Active tab/menu: Primary Blue background, White text
- Inactive tab/menu: Surface Dark background, Light Gray text
- Hover: Dark Gray background
- Disabled: Medium Gray text

## QSS Implementation

Below are code snippets for implementing the dark mode design using Qt Style Sheets (QSS):

### Global Application Style

```css
QWidget {
    font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
    font-size: 12px;
    color: #FFFFFF;
    background-color: #121212;
}

QMainWindow, QDialog {
    background: #121212;
}

QLabel {
    color: #FFFFFF;
}

QLabel#heading {
    font-size: 18px;
    font-weight: bold;
    color: #FFFFFF;
}

QLabel#subheading {
    font-size: 16px;
    font-weight: bold;
    color: #FFFFFF;
}

/* Tooltip styling */
QToolTip {
    border: 1px solid #2D2D2D;
    background-color: #1E1E1E;
    color: #FFFFFF;
    padding: 4px;
}
```

### Button Styles

```css
QPushButton {
    background-color: #2979FF;
    color: white;
    border: none;
    border-radius: 4px;
    padding: 8px 16px;
    min-width: 80px;
}

QPushButton:hover {
    background-color: #5393FF;
}

QPushButton:pressed {
    background-color: #0D47A1;
}

QPushButton:disabled {
    background-color: #2D2D2D;
    color: #757575;
}

QPushButton.secondary {
    background-color: #2D2D2D;
    color: #FFFFFF;
    border: 1px solid #2979FF;
}

QPushButton.secondary:hover {
    background-color: rgba(41, 121, 255, 0.2);
}

QPushButton.success {
    background-color: #43A047;
}

QPushButton.success:hover {
    background-color: #2E7D32;
}

QPushButton.danger {
    background-color: #E53935;
}

QPushButton.danger:hover {
    background-color: #C62828;
}
```

### Input Field Styles

```css
QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {
    border: 1px solid #757575;
    border-radius: 4px;
    padding: 8px;
    background: #2D2D2D;
    color: #FFFFFF;
    selection-background-color: #5393FF;
}

QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {
    border: 1px solid #2979FF;
}

QLineEdit:disabled, QTextEdit:disabled, QPlainTextEdit:disabled, QSpinBox:disabled, QComboBox:disabled {
    background: #1E1E1E;
    color: #757575;
}

QComboBox {
    padding-right: 20px; /* For the dropdown arrow */
}

QComboBox::drop-down {
    subcontrol-origin: padding;
    subcontrol-position: right center;
    width: 20px;
    border-left: 1px solid #757575;
}

QComboBox QAbstractItemView {
    border: 1px solid #757575;
    background-color: #1E1E1E;
    selection-background-color: #5393FF;
}
```

### Table and List Styles

```css
QTableView, QListView, QTreeView {
    border: 1px solid #2D2D2D;
    background-color: #121212;
    selection-background-color: rgba(41, 121, 255, 0.3);
    selection-color: white;
    alternate-background-color: #1E1E1E;
    gridline-color: #2D2D2D;
}

QTableView::item, QListView::item, QTreeView::item {
    padding: 6px;
    color: #FFFFFF;
}

QTableView::item:hover, QListView::item:hover, QTreeView::item:hover {
    background-color: #2D2D2D;
}

QHeaderView::section {
    background-color: #2D2D2D;
    color: #BBBBBB;
    padding: 6px;
    border: none;
    font-weight: bold;
}

QTableView::item:selected, QListView::item:selected, QTreeView::item:selected {
    background-color: rgba(41, 121, 255, 0.3);
    color: white;
}
```

### Tab Widget Styles

```css
QTabWidget::pane {
    border: 1px solid #2D2D2D;
    background-color: #1E1E1E;
    padding: 5px;
}

QTabBar::tab {
    background: #1E1E1E;
    color: #BBBBBB;
    padding: 8px 16px;
    border: 1px solid #2D2D2D;
    border-bottom: none;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
}

QTabBar::tab:selected {
    background: #2979FF;
    color: white;
}

QTabBar::tab:hover:!selected {
    background: #2D2D2D;
}
```

### Menu Styles

```css
QMenuBar {
    background-color: #1E1E1E;
    color: #FFFFFF;
    border-bottom: 1px solid #2D2D2D;
}

QMenuBar::item {
    padding: 8px 12px;
}

QMenuBar::item:selected, QMenu::item:selected {
    background-color: #2979FF;
    color: white;
}

QMenu {
    background-color: #1E1E1E;
    border: 1px solid #2D2D2D;
}

QMenu::item {
    padding: 6px 20px 6px 20px;
    color: #FFFFFF;
}

QMenu::separator {
    height: 1px;
    background: #2D2D2D;
    margin: 4px 0px;
}
```

### Scroll Bar Styles

```css
QScrollBar:vertical {
    border: none;
    background: #2D2D2D;
    width: 10px;
    margin: 0px;
}

QScrollBar::handle:vertical {
    background: #757575;
    min-height: 20px;
    border-radius: 5px;
}

QScrollBar::handle:vertical:hover {
    background: #2979FF;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    border: none;
    background: #2D2D2D;
    height: 10px;
    margin: 0px;
}

QScrollBar::handle:horizontal {
    background: #757575;
    min-width: 20px;
    border-radius: 5px;
}

QScrollBar::handle:horizontal:hover {
    background: #2979FF;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}
```

## Component-Specific Styling

### Product List Item
```css
ProductListItem {
    background: #1E1E1E;
    border: 1px solid #2D2D2D;
    border-radius: 4px;
    padding: 10px;
    margin: 5px;
}

ProductListItem:hover {
    border-color: #2979FF;
    box-shadow: 0 4px 8px rgba(0,0,0,0.5);
}

ProductListItem .title {
    font-weight: bold;
    color: #FFFFFF;
    font-size: 14px;
}

ProductListItem .price {
    font-weight: bold;
    color: #2979FF;
}

ProductListItem .rating {
    color: #FB8C00;
}
```

### Shopping Cart Item
```css
CartItem {
    background: #1E1E1E;
    border-bottom: 1px solid #2D2D2D;
    padding: 10px;
}

CartItem .quantity {
    font-weight: bold;
    color: #FFFFFF;
}

CartItem .remove-button {
    border: none;
    background: transparent;
    color: #E53935;
    font-weight: bold;
}
```

### Order Status Indicators
```css
QLabel.status-pending {
    color: #FB8C00;
    font-weight: bold;
}

QLabel.status-processing {
    color: #2979FF;
    font-weight: bold;
}

QLabel.status-shipped {
    color: #43A047;
    font-weight: bold;
}

QLabel.status-delivered {
    color: #43A047;
    font-weight: bold;
}

QLabel.status-cancelled {
    color: #E53935;
    font-weight: bold;
}
```

## Application of Styles

To apply these styles in the Qt application:

1. Create a `styles.qss` file containing all the style definitions.
2. Load the stylesheet in the application's main function:

```cpp
QFile styleFile(":/styles/styles.qss");
styleFile.open(QFile::ReadOnly);
QString styleSheet = QLatin1String(styleFile.readAll());
qApp->setStyleSheet(styleSheet);
```

3. For component-specific styles, either use object names or specify the class directly in the stylesheet.

4. To set object-specific styles:
```cpp
// Setting object name for CSS targeting
QPushButton* addButton = new QPushButton("Add to Cart");
addButton->setObjectName("add-to-cart-button");
// OR
// Setting class for CSS targeting
addButton->setProperty("class", "success");
```

This dark mode styling approach provides a consistent, modern, and visually appealing user interface that aligns with the UX principles outlined in `ux_design.md`. The dark color scheme reduces eye strain during extended use and gives the application a premium, contemporary feel. 