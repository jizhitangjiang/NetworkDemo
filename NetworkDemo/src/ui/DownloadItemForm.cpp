#include "DownloadItemForm.h"
#include "ui_DownloadItemForm.h"

#include <QFile>
#include <QTimer>
#include "Utils.h"

DownloadItemForm::DownloadItemForm(QString url, QString filePath, QWidget *parent)
    : QWidget(parent)
    , m_network(nullptr)
    , m_rid(-1)
    , m_recevieBytes(0)
    , m_url(url)
    , m_filePath(filePath)
    , ui(new Ui::DownloadItemForm)
{
    ui->setupUi(this);

    init();
}

DownloadItemForm::~DownloadItemForm()
{
    delete ui;
}

void DownloadItemForm::download()
{
    if (m_url.isEmpty() || m_filePath.isEmpty()) {
        return;
    }

    ui->btnStart->setText("暂停");
    m_rid = m_network->downloadFile(m_url, m_filePath);
    m_time.start();
}

void DownloadItemForm::init()
{
    QUrl url(m_url);
    ui->labelName->setText(url.fileName());

    m_network = Utils::instance()->network();
    connect(m_network, &NetWork::downloadProgress, this, &DownloadItemForm::onDownloadProgress);
    connect(m_network, &NetWork::requestFinished, this, &DownloadItemForm::onDownloadFinished);
}

qint64 DownloadItemForm::downloadSpeed(qint64 receiveBytes)
{
    if (receiveBytes < m_recevieBytes) {
        return 0;
    }

    qint64 inervalBytes = receiveBytes - m_recevieBytes;
    int elapsedTime = m_time.elapsed();
    qint64 speed = inervalBytes * 1000 / elapsedTime;
    m_time.restart();

    m_recevieBytes = receiveBytes;
    return speed;
}

QString DownloadItemForm::remainTime(qint64 remainBytes, qint64 speed)
{
    if (speed <= 0) {
        return "";
    }

    qint64 seconds = remainBytes / speed;
    QTime time(0, 0, 0);
    time = time.addSecs(seconds);

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
    ui->labelSpeed->setText(QString("%1/s").arg(Utils::instance()->trasformUnit(speed)));
    ui->labelTime->setText(remainTime(bytesTotal - bytesReceived, speed));
    ui->labelSize->setText(QString("%1/%2")
        .arg(Utils::instance()->trasformUnit(bytesReceived))
        .arg(Utils::instance()->trasformUnit(bytesTotal)));

    update();
}

void DownloadItemForm::onDownloadFinished(int rid, qint64 fileSize)
{
    if (rid != m_rid) {
        return;
    }

    ui->labelSize->setText(Utils::instance()->trasformUnit(fileSize));
    ui->labelInfo->setText("文件下载完成");
    ui->labelSpeed->hide();
    ui->labelTime->hide();
    ui->btnStart->hide();
    ui->btnStop->hide();
}


void DownloadItemForm::on_btnStart_clicked(bool checked)
{
    if (checked) {
        download();
    } else {
        ui->btnStart->setText("开始");
        m_network->stop(m_rid);
        m_rid = -1;
        m_recevieBytes = 0;
    }
}

void DownloadItemForm::on_btnStop_clicked()
{

}
