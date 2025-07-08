#ifndef STYLEDEMOWIDGET_H
#define STYLEDEMOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QScrollArea>

class StyleDemoWidget : public QWidget {
    Q_OBJECT

public:
    explicit StyleDemoWidget(QWidget *parent = nullptr);

private:
    QWidget* createButtonsGroup();
    QWidget* createInputsGroup();
    QWidget* createTableGroup();
    QWidget* createStatusGroup();
};

#endif // STYLEDEMOWIDGET_H 