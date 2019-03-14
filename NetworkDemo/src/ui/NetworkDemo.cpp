#include "NetworkDemo.h"
#include "ui_NetworkDemo.h"

#include <QDebug>

NetworkDemo::NetworkDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkDemo)
{
    ui->setupUi(this);

    init();
}

NetworkDemo::~NetworkDemo()
{
    delete ui;
}

void NetworkDemo::setTrayIcon(SystemTrayIcon *tray)
{
    if (tray == NULL) {
        return;
    }

    connect(tray, &SystemTrayIcon::trayEventActivate,
        this, &NetworkDemo::onTrayEventActivate);
}

void NetworkDemo::init()
{
    //setWindowFlags(Qt::FramelessWindowHint);
}

void NetworkDemo::onTrayEventActivate(SystemTrayIcon::TrayEventType type)
{
    switch(type) {
    case SystemTrayIcon::TrayEventType_Open:
        window()->activateWindow();
        showNormal();
        break;
    case SystemTrayIcon::TrayEventType_Minimun:
        showMinimized();
        break;
    case SystemTrayIcon::TrayEventType_Close:
        close();
        break;
    }
}


