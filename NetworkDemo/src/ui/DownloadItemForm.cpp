﻿#include "DownloadItemForm.h"
#include "ui_DownloadItemForm.h"

#include <QFile>
#include "Utils.h"

#define SIZE_KB 1024
#define SIZE_MB 1024 * 1024
#define SIZE_GB 1024 * 1024 *1024

DownloadItemForm::DownloadItemForm(QWidget *parent)
    : QWidget(parent)
    , m_network(NULL)
    , m_rid(-1)
    , m_recevieBytes(0)
    , ui(new Ui::DownloadItemForm)
{
    ui->setupUi(this);

    init();
}

DownloadItemForm::~DownloadItemForm()
{
    delete ui;
}

void DownloadItemForm::init()
{
    m_network = Utils::instance()->network();
    connect(m_network, &NetWork::downloadProgress, this, &DownloadItemForm::onDownloadProgress);
    connect(m_network, &NetWork::requestFinished, this, &DownloadItemForm::onDownloadFinished);
}

QString DownloadItemForm::trasformUnit(qint64 bytes)
{
    QString unit = "B";
    if (bytes < SIZE_KB) {

    } else if (bytes < SIZE_MB) {
        bytes /= SIZE_KB;
        unit = "KB";
    } else if (bytes < SIZE_GB) {
        bytes /= SIZE_MB;
        unit = "MB";
    } else {
        bytes /= SIZE_GB;
        unit = "GB";
    }

    return  QString("%1%2").arg(bytes).arg(unit);
}

qint64 DownloadItemForm::downloadSpeed(qint64 receiveBytes)
{
    if (receiveBytes < m_recevieBytes) {
        return 0;
    }

    qint64 inervalBytes = receiveBytes - m_recevieBytes;
    m_recevieBytes = receiveBytes;
    qint64 speed = inervalBytes * 1000 / m_time.elapsed();
    m_time.restart();

    return speed;
}

QString DownloadItemForm::remainTime(qint64 remainBytes, qint64 speed)
{
    qint64 seconds = remainBytes / speed;
    QTime time(0, 0, 0);
    time = time.addSecs(seconds);
    qDebug()<<"seconds"<<seconds;
    return time.toString("hh:mm:ss");
}

void DownloadItemForm::onDownloadProgress(int rid, qint64 bytesReceived, qint64 bytesTotal)
{
    if (rid != m_rid || bytesTotal < bytesReceived) {
        return;
    }

    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesReceived);

    qint64 speed = downloadSpeed(bytesReceived);
    ui->labelSpeed->setText(QString("%1/s").arg(trasformUnit(speed)));
    ui->labelTime->setText(remainTime(bytesTotal - bytesReceived, speed));
    ui->labelSize->setText(QString("%1/%2")
        .arg(trasformUnit(bytesReceived))
        .arg(trasformUnit(bytesTotal)));

}

void DownloadItemForm::onDownloadFinished(int rid, qint64 fileSize)
{
    if (rid != m_rid) {
        return;
    }

    ui->labelSize->setText(trasformUnit(fileSize));
    ui->labelInfo->setText("文件下载完成");
    ui->labelSpeed->hide();
    ui->labelTime->hide();
    ui->btnStart->hide();
    ui->btnStop->hide();
    ui->progressBar->hide();
}

void DownloadItemForm::on_btnStop_clicked(bool checked)
{
    if (checked) {
        m_rid = m_network->downloadFile("http://mv.xesimg.com/martrix/web/message/1551861572464/%E5%BA%8F%E5%88%9701_1~1.mp4","D://assist.mp4");
        m_time.start();
    } else {
        m_network->stop(m_rid);
        m_rid = -1;
        m_recevieBytes = 0;
    }
}
