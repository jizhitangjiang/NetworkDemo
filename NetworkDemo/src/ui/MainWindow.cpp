﻿#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QDebug>
#include <QJsonObject>

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

    m_windowStyle = new WindowStyle(this);
    m_windowStyle->activateOn(this);

    m_hostInfo = new HostInfo(this);
    ui->plainTextEdit->appendPlainText("本地主机名：" + m_hostInfo->hostName());
    ui->plainTextEdit->appendPlainText("主机IP地址列表:\n" + m_hostInfo->hostAddress());

    connect(m_hostInfo, &HostInfo::lookupHostFinish, this, &MainWindow::onLookupHostFinished);

    DownloadItemForm *item= new DownloadItemForm(this);
    ui->widgetContent->layout()->addWidget(item);
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
