#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QDebug>
#include <QJsonObject>
#include <QGraphicsDropShadowEffect>

#include "Utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_windowStyle(NULL)
    , m_hostInfo(NULL)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTrayIcon(SystemTrayIcon *tray)
{
    if (tray == NULL) {
        return;
    }

    connect(tray, &SystemTrayIcon::trayEventActivate,
        this, &MainWindow::onTrayEventActivate);
}

void MainWindow::init()
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    m_windowStyle = new WindowStyle(this);
    m_windowStyle->activateOn(ui->widgetTitle);

    m_hostInfo = new HostInfo(this);
    ui->plainTextEdit->appendPlainText("本地主机名：" + m_hostInfo->hostName());
    ui->plainTextEdit->appendPlainText("主机IP地址列表:\n" + m_hostInfo->hostAddress());
    connect(m_hostInfo, &HostInfo::lookupHostFinish, this, &MainWindow::onLookupHostFinished);

//    for (int i = 0; i < 10 ;i++) {
//        ui->widgetList->createDownItemForm("http://mv.xesimg.com/martrix/web/message/1551861572464/%E5%BA%8F%E5%88%9701_1~1.mp4",
//                                           QString("D://assist%1.mp4").arg(i));
//    }
}

void MainWindow::onTrayEventActivate(SystemTrayIcon::TrayEventType type)
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

void MainWindow::on_btnClose_clicked()
{
    close();
}

void MainWindow::on_btnMin_clicked()
{
    showMinimized();
}

void MainWindow::onLookupHostFinished(bool isSucessed,
    const QJsonObject &obj,
    const QString &error)
{
    if (!isSucessed) {
        ui->plainTextEdit->appendPlainText("错误:" + error);
        return;
    }

    ui->plainTextEdit->appendPlainText(obj.value("host").toString() + ":");
    ui->plainTextEdit->appendPlainText(obj.value("ip").toString());
}

void MainWindow::on_btnDomainIp_clicked()
{
    QString domain = ui->lineEdit->text();
    if (!domain.isEmpty()) {
        m_hostInfo->getDomainAddress(domain);
    }
}
