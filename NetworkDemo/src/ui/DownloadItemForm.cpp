#include "DownloadItemForm.h"
#include "ui_DownloadItemForm.h"

#include <QFile>


DownloadItemForm::DownloadItemForm(QWidget *parent)
    : QWidget(parent)
    , m_network(NULL)
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
    m_network = new NetWork(this);
}

void DownloadItemForm::on_btnStart_clicked()
{
    m_network->downloadFile("http://client.xesimg.com/oa/win/4.4/xes_assist_win_setup_4.4.5.0.exe","D://assist.exe");
  // qDebug()<< QFileDialog::getSaveFileName();
  // qDebug()<< QFileDialog::getSaveFileUrl();
}

void DownloadItemForm::onDownloadProgress(int rid, qint64 bytesReceived, qint64 bytesTotal)
{

}
