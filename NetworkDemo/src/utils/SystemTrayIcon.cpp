#include "SystemTrayIcon.h"

#include <QMenu>
#include <QApplication>

SystemTrayIcon::SystemTrayIcon(QObject *parent)
    : QObject(parent)
{
    init();
}

SystemTrayIcon::~SystemTrayIcon()
{
    m_trayIcon.hide();
}

void SystemTrayIcon::init()
{
    m_trayIcon.setIcon(QIcon(":/res/image/app.ico"));
    m_trayIcon.setToolTip("下载工具");

    QMenu *menu = new QMenu((QWidget*)QApplication::desktop());
    QAction *openAction = new QAction("打开下载工具", this);
    QAction *minAction = new QAction("最小化", this);
    QAction *closeAction = new QAction("关闭", this);
    openAction->setProperty("key", TrayEventType_Open);
    minAction->setProperty("key", TrayEventType_Minimun);
    closeAction->setProperty("key", TrayEventType_Close);
    menu->addAction(openAction);
    menu->addAction(minAction);
    menu->addAction(closeAction);

    connect(menu, &QMenu::triggered, this, &SystemTrayIcon::onActionClicked);

    m_trayIcon.setContextMenu(menu);
    m_trayIcon.show();
}

void SystemTrayIcon::onActionClicked(QAction *action)
{
    int value = action->property("key").toInt();
    if (value < TrayEventType_Open || value > TrayEventType_Close) {
        return;
    }
    TrayEventType type = (TrayEventType)value;
    emit trayEventActivate(type);
}
