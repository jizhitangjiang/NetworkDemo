#include "DownloadItemForm.h"
#include "ui_DownloadItemForm.h"

#include <QFile>
#include "Utils.h"

DownloadItemForm::DownloadItemForm(QWidget *parent)
    : QWidget(parent)
    , m_network(NULL)
    , m_rid(-1)
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

void DownloadItemForm::onDownloadProgress(int rid, qint64 bytesReceived, qint64 bytesTotal)
{
    if (rid != m_rid || bytesTotal < bytesReceived) {
        return;
    }

    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesReceived);
    qDebug()<<bytesTotal;
    qDebug()<<bytesReceived;
}

void DownloadItemForm::onDownloadFinished(int rid)
{
    if (rid != m_rid) {
        return;
    }

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
        m_rid = m_network->downloadFile("thunder://QUFmdHA6Ly8zOjNAZGhkLmRsMTIzNC5jb206ODAwNi8lRTclQkIlOUQlRTUlOTElQkQlRTYlQUYlOTIlRTUlQjglODglRTclQUMlQUMlRTQlQjglODklRTUlQUQlQTMxMyVFOSU5QiU4NkJEJUU0JUI4JUFEJUU4JThCJUIxJUU1JThGJThDJUU1JUFEJTk3WyVFNyU5NCVCNSVFNSVCRCVCMSVFNSVBNCVBOSVFNSVBMCU4Mnd3dy5keTIwMTguY29tXS5ybXZiWlo=","D://assist.exe");
    } else {
        m_network->stop(m_rid);
        m_rid = -1;
    }
}
