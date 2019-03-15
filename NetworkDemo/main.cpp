#include "MainWindow.h"
#include <QApplication>

#include "Utils.h"
#include "SystemTrayIcon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(Utils::instance()->getAppCss());

    MainWindow w;
    SystemTrayIcon icon;
    w.setTrayIcon(&icon);

    w.show();

    return a.exec();
}
