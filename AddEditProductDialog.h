#ifndef ADDEDITPRODUCTDIALOG_H
#define ADDEDITPRODUCTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit> // For description
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QString>
// Removed CartTypes.h include

// Direct implementation of ProductDetails class
class ProductDetails {
private:
    int m_id;
    QString m_name;
    double m_price;
    QString m_category;
    int m_stock;
    QString m_description;

public:
    ProductDetails() : m_id(0), m_price(0.0), m_stock(0) {}
    
    // Getters
    int getId() const { return m_id; }
    const QString& getName() const { return m_name; }
    double getPrice() const { return m_price; }
    const QString& getCategory() const { return m_category; }
    int getStock() const { return m_stock; }
    const QString& getDescription() const { return m_description; }
    
    // Setters
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setPrice(double price) { m_price = price; }
    void setCategory(const QString& category) { m_category = category; }
    void setStock(int stock) { m_stock = stock; }
    void setDescription(const QString& description) { m_description = description; }
};

class AddEditProductDialog : public QDialog
{
    Q_OBJECT

public:
    inline explicit AddEditProductDialog(QWidget *parent = nullptr);
    inline explicit AddEditProductDialog(const ProductDetails &productToEdit, QWidget *parent = nullptr);
    inline ~AddEditProductDialog();

    inline ProductDetails getProductDetails() const;

private:
    inline void setupUi();
    inline void populateFields(const ProductDetails &product);

    QLineEdit *nameLineEdit;
    QDoubleSpinBox *priceSpinBox;
    QLineEdit *categoryLineEdit;
    QSpinBox *stockSpinBox;
    QTextEdit *descriptionTextEdit;
    QDialogButtonBox *buttonBox;

    int currentProductId = 0; // To store ID when editing
};

inline AddEditProductDialog::AddEditProductDialog(QWidget *parent)
    : QDialog(parent), currentProductId(0) // For adding new product
{
    setupUi();
    setWindowTitle("Add New Product");
}

inline AddEditProductDialog::AddEditProductDialog(const ProductDetails &productToEdit, QWidget *parent)
    : QDialog(parent), currentProductId(productToEdit.getId())
{
    setupUi();
    setWindowTitle("Edit Product");
    populateFields(productToEdit);
}

inline AddEditProductDialog::~AddEditProductDialog() {}

inline void AddEditProductDialog::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    nameLineEdit = new QLineEdit(this);
    priceSpinBox = new QDoubleSpinBox(this);
    priceSpinBox->setRange(0.0, 99999.99);
    priceSpinBox->setDecimals(2);
    priceSpinBox->setPrefix("$");
    categoryLineEdit = new QLineEdit(this);
    stockSpinBox = new QSpinBox(this);
    stockSpinBox->setRange(0, 99999);
    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setAcceptRichText(false);
    descriptionTextEdit->setPlaceholderText("Enter product description here...");
    descriptionTextEdit->setFixedHeight(100);

    formLayout->addRow("Name:", nameLineEdit);
    formLayout->addRow("Price:", priceSpinBox);
    formLayout->addRow("Category:", categoryLineEdit);
    formLayout->addRow("Stock:", stockSpinBox);
    formLayout->addRow("Description:", descriptionTextEdit);

    mainLayout->addLayout(formLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setMinimumWidth(400);
}

inline void AddEditProductDialog::populateFields(const ProductDetails &product) {
    nameLineEdit->setText(product.getName());
    priceSpinBox->setValue(product.getPrice());
    categoryLineEdit->setText(product.getCategory());
    stockSpinBox->setValue(product.getStock());
    descriptionTextEdit->setPlainText(product.getDescription());
    currentProductId = product.getId();
}

inline ProductDetails AddEditProductDialog::getProductDetails() const {
    ProductDetails details;
    details.setId(currentProductId); // Will be 0 for new, or existing ID for edit
    details.setName(nameLineEdit->text());
    details.setPrice(priceSpinBox->value());
    details.setCategory(categoryLineEdit->text());
    details.setStock(stockSpinBox->value());
    details.setDescription(descriptionTextEdit->toPlainText());
    return details;
}

#endif // ADDEDITPRODUCTDIALOG_H 