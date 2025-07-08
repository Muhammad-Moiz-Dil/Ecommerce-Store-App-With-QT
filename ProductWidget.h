#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QMouseEvent> // Required for mousePressEvent

// Forward declaration if Product.h is complex or to avoid circular dependencies.
// For now, we assume Product data is passed as basic types.
// class Product; 

class ProductWidget : public QWidget
{
    Q_OBJECT

public:
    inline explicit ProductWidget(int id, const QString &name, double price, const QString &category, double rating, QWidget *parent = nullptr)
        : QWidget(parent), productID(id)
    {
        setAttribute(Qt::WA_StyledBackground, true); // Explicitly enable styled background
        QVBoxLayout *layout = new QVBoxLayout(this);

        nameLabel = new QLabel(QString("<b>Name:</b> %1").arg(name), this);
        priceLabel = new QLabel(QString("<b>Price:</b> $%1").arg(price, 0, 'f', 2), this); // Format price to 2 decimal places
        categoryLabel = new QLabel(QString("<b>Category:</b> %1").arg(category), this);
        ratingLabel = new QLabel(QString("<b>Rating:</b> %1/5.0").arg(rating, 0, 'f', 1), this); // Format rating to 1 decimal place

        layout->addWidget(nameLabel);
        layout->addWidget(priceLabel);
        layout->addWidget(categoryLabel);
        layout->addWidget(ratingLabel);
        
        setLayout(layout);

        // Basic styling to make it look clickable (optional)
        setCursor(Qt::PointingHandCursor);
        setStyleSheet(
            "ProductWidget { "
            /* Remove explicit background-color to inherit system theme */
            "    border: 1px solid #555555; /* Darker gray border, visible on dark themes */ "
            "    padding: 5px; "
            "    margin: 3px; "
            "    border-radius: 3px; "
            "} "
            "ProductWidget QLabel { "
            "    color: white; "
            "    background-color: transparent; "
            "} "
            "ProductWidget:hover { "
            "    background-color: rgba(255, 255, 255, 0.1); /* Subtle white highlight on hover */ "
            "}"
        );
    }

    inline int getProductID() const {
        return productID;
    }

signals:
    void productClicked(int productID);

protected:
    inline void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            emit productClicked(productID);
        }
        QWidget::mousePressEvent(event); // Call base class implementation
    }

private:
    int productID;
    QLabel *nameLabel;
    QLabel *priceLabel;
    QLabel *categoryLabel;
    QLabel *ratingLabel;
};

#endif // PRODUCTWIDGET_H 