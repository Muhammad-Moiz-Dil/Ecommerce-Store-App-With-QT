#ifndef REVIEWWIDGET_H
#define REVIEWWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <fstream>
#include <sstream>
#include <QDir>
#include "../../include/Review.h"

class ReviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReviewWidget(int productId, int userId, QWidget *parent = nullptr);
    ~ReviewWidget();

signals:
    void reviewAdded(int productId); // Signal emitted when a review is added

private slots:
    void onSubmitReview();

private:
    void setupUI();
    void loadReviews();
    void updateReviewEligibility();

    int productId;
    int userId;
    
    QListWidget *reviewsListWidget;
    QComboBox *ratingComboBox;
    QTextEdit *commentTextEdit;
    QPushButton *submitButton;
};

#endif // REVIEWWIDGET_H 