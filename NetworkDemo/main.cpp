#include "NetworkDemo.h"
#include <QApplication>

#include "Utils.h"
#include "SystemTrayIcon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(Utils::instance()->getAppCss());

    NetworkDemo w;
  //  SystemTrayIcon icon;
  //  w.setTrayIcon(&icon);

    w.show();

    int ret = a.exec();
    return ret;
}
