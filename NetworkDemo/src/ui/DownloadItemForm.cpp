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

}

void DownloadItemForm::onDownloadProgress(int rid, qint64 bytesReceived, qint64 bytesTotal)
{

}
