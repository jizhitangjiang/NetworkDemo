#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "WindowStyle.h"
#include "SystemTrayIcon.h"
#include "HostInfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /**
      * @brief 设置托盘图标
      * @param tray 托盘图标
      */
     void setTrayIcon(SystemTrayIcon *tray);

private:
    void init();

private slots:
    void onTrayEventActivate(SystemTrayIcon::TrayEventType type);
    void on_btnClose_clicked();
    void on_btnMin_clicked();
    void onLookupHostFinished(bool isSucessed,
        const QJsonObject &obj,
        const QString &error);
    void on_btnDomainIp_clicked();

private:
    Ui::MainWindow *ui;
    WindowStyle *m_windowStyle;
    HostInfo    *m_hostInfo;
};

#endif // MAINWINDOW_H
