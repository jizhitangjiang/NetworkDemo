#ifndef NETWORKDEMO_H
#define NETWORKDEMO_H

#include <QWidget>

#include "SystemTrayIcon.h"

namespace Ui {
class NetworkDemo;
}

class NetworkDemo : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkDemo(QWidget *parent = 0);
    ~NetworkDemo();

    /**
      * @brief 设置托盘图标
      * @param tray 托盘图标
      */
     void setTrayIcon(SystemTrayIcon *tray);

private:
    void init();


private slots:
    void onTrayEventActivate(SystemTrayIcon::TrayEventType type);

private:
    Ui::NetworkDemo *ui;

};

#endif // NETWORKDEMO_H
