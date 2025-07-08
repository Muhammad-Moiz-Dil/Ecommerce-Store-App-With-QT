#include <QApplication>
#include "gui/include/MainWindow.h"
#include "src/StyleManager.h"
#include <QTimer>
#include <QFile>
#include <QString>
#include <QDir>
#include <QStyle>
#include <QStyleFactory>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QApplication::setApplicationName("E-Commerce Application");
    QApplication::setOrganizationName("YourOrganization");
    QApplication::setApplicationVersion("0.69420");

        
    app.setStyle(QStyleFactory::create("Fusion"));
    
  
    app.setAttribute(Qt::AA_DontShowIconsInMenus);
    
    StyleManager& styleManager = StyleManager::getInstance();
    if (styleManager.loadStylesheet("styles.qss")) {
        app.setStyleSheet(QString::fromStdString(styleManager.getStylesheet()));
    } else {
        QFile styleFile("styles.qss");
        if (styleFile.open(QFile::ReadOnly)) {
            app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
            styleFile.close();
        } else {
            qWarning("Failed to load stylesheet file.");
        }
    }

    MainWindow mainWindow;
    mainWindow.show();

    QTimer::singleShot(0, &mainWindow, SLOT(showLoginDialog()));

    return app.exec();
} 