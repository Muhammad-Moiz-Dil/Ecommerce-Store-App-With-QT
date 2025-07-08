#include "../include/StyleDemoWidget.h"

StyleDemoWidget::StyleDemoWidget(QWidget *parent) : QWidget(parent) {
    // Set main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Add heading
    QLabel *heading = new QLabel("Dark Mode Style Demo", this);
    heading->setObjectName("heading");
    heading->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(heading);
    
    // Add subheading with description
    QLabel *subheading = new QLabel("This widget demonstrates the styling components available in the application", this);
    subheading->setObjectName("subheading");
    subheading->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subheading);
    
    // Create scroll area for all demo components
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    QWidget *scrollContent = new QWidget(scrollArea);
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    
    // Add the component groups to the scroll layout
    scrollLayout->addWidget(createButtonsGroup());
    scrollLayout->addWidget(createInputsGroup());
    scrollLayout->addWidget(createTableGroup());
    scrollLayout->addWidget(createStatusGroup());
    
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

QWidget* StyleDemoWidget::createButtonsGroup() {
    QGroupBox *groupBox = new QGroupBox("Button Styles", this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    
    // Standard button
    QPushButton *standardButton = new QPushButton("Primary Button", this);
    layout->addWidget(standardButton);
    
    // Secondary button
    QPushButton *secondaryButton = new QPushButton("Secondary Button", this);
    secondaryButton->setProperty("class", "secondary");
    layout->addWidget(secondaryButton);
    
    // Success button
    QPushButton *successButton = new QPushButton("Success Button", this);
    successButton->setProperty("class", "success");
    layout->addWidget(successButton);
    
    // Danger button
    QPushButton *dangerButton = new QPushButton("Danger Button", this);
    dangerButton->setProperty("class", "danger");
    layout->addWidget(dangerButton);
    
    // Disabled button
    QPushButton *disabledButton = new QPushButton("Disabled Button", this);
    disabledButton->setEnabled(false);
    layout->addWidget(disabledButton);
    
    return groupBox;
}

QWidget* StyleDemoWidget::createInputsGroup() {
    QGroupBox *groupBox = new QGroupBox("Input Styles", this);
    QFormLayout *layout = new QFormLayout(groupBox);
    
    // Text input
    QLineEdit *textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text here...");
    layout->addRow("Text Input:", textInput);
    
    // Disabled text input
    QLineEdit *disabledInput = new QLineEdit(this);
    disabledInput->setPlaceholderText("Disabled input");
    disabledInput->setEnabled(false);
    layout->addRow("Disabled Input:", disabledInput);
    
    // Combo box
    QComboBox *comboBox = new QComboBox(this);
    comboBox->addItem("Option 1");
    comboBox->addItem("Option 2");
    comboBox->addItem("Option 3");
    layout->addRow("Combo Box:", comboBox);
    
    // Checkbox
    QCheckBox *checkbox = new QCheckBox("Checkbox Option", this);
    layout->addRow("Checkbox:", checkbox);
    
    // Radio buttons
    QWidget *radioContainer = new QWidget(this);
    QHBoxLayout *radioLayout = new QHBoxLayout(radioContainer);
    QRadioButton *radio1 = new QRadioButton("Option 1", this);
    QRadioButton *radio2 = new QRadioButton("Option 2", this);
    radioLayout->addWidget(radio1);
    radioLayout->addWidget(radio2);
    layout->addRow("Radio Buttons:", radioContainer);
    
    return groupBox;
}

QWidget* StyleDemoWidget::createTableGroup() {
    QGroupBox *groupBox = new QGroupBox("Tables and Lists", this);
    QVBoxLayout *layout = new QVBoxLayout(groupBox);
    
    // Create a table widget
    QTableWidget *table = new QTableWidget(5, 3, this);
    table->setHorizontalHeaderLabels({"Column 1", "Column 2", "Column 3"});
    table->setAlternatingRowColors(true);
    
    // Add some sample data
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 3; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString("Item %1,%2").arg(row).arg(col));
            table->setItem(row, col, item);
        }
    }
    
    // Set a reasonable size for the demo
    table->setMaximumHeight(150);
    
    layout->addWidget(table);
    
    return groupBox;
}

QWidget* StyleDemoWidget::createStatusGroup() {
    QGroupBox *groupBox = new QGroupBox("Status Indicators", this);
    QFormLayout *layout = new QFormLayout(groupBox);
    
    // Pending status
    QLabel *pendingLabel = new QLabel("Pending", this);
    pendingLabel->setProperty("class", "status-pending");
    layout->addRow("Pending:", pendingLabel);
    
    // Processing status
    QLabel *processingLabel = new QLabel("Processing", this);
    processingLabel->setProperty("class", "status-processing");
    layout->addRow("Processing:", processingLabel);
    
    // Shipped status
    QLabel *shippedLabel = new QLabel("Shipped", this);
    shippedLabel->setProperty("class", "status-shipped");
    layout->addRow("Shipped:", shippedLabel);
    
    // Delivered status
    QLabel *deliveredLabel = new QLabel("Delivered", this);
    deliveredLabel->setProperty("class", "status-delivered");
    layout->addRow("Delivered:", deliveredLabel);
    
    // Cancelled status
    QLabel *cancelledLabel = new QLabel("Cancelled", this);
    cancelledLabel->setProperty("class", "status-cancelled");
    layout->addRow("Cancelled:", cancelledLabel);
    
    return groupBox;
} 