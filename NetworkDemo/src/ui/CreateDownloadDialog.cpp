#include "CreateDownloadDialog.h"
#include "ui_CreateDownloadDialog.h"

#include "windows.h"

#include <QDebug>
#include <QUrl>
#include <QNetworkReply>
#include <QFileInfo>
#include <QRegExp>
#include <QFileDialog>
#include <QStandardItemModel>

#include "Utils.h"

CreateDownloadDialog::CreateDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDownloadDialog)
{
    ui->setupUi(this);

    init();
}

CreateDownloadDialog::~CreateDownloadDialog()
{
    delete ui;
}

void CreateDownloadDialog::getDownloadInfo(QString &url, QString &path)
{
    url = m_url.url();
    path = m_path + "/" + m_url.fileName();
}

void CreateDownloadDialog::init()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    setDirectoryInfo("C://");

    QStandardItemModel *model = new QStandardItemModel;
    ui->tableView->setModel(model);
    model->setHorizontalHeaderItem(0, new QStandardItem("文件名"));
    model->setHorizontalHeaderItem(1, new QStandardItem("类型"));
    model->setHorizontalHeaderItem(2, new QStandardItem("大小"));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_timer.setInterval(1000);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &CreateDownloadDialog::onTimeOut);
}

qint64 CreateDownloadDialog::getDiskFreeSize(QString path)
{
    LPCWSTR lpcwstrDriver = (LPCWSTR)path.utf16();

       ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;

       if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
       {
           qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
           return 0;
       }
       return (quint64)(liTotalFreeBytes.QuadPart);
}

void CreateDownloadDialog::setDirectoryInfo(const QString &dir)
{
    ui->lineEdit->setText(dir);

    qint64 size = getDiskFreeSize(dir);
    ui->label->setText(Utils::instance()->trasformUnit(size));
}

void CreateDownloadDialog::on_btnClose_clicked()
{
    reject();
}

void CreateDownloadDialog::on_textEdit_textChanged()
{
    m_timer.stop();

    QString url = ui->textEdit->toPlainText();
    m_url.setUrl(url);

    if (!m_url.isValid()) {
        return;
    }

    m_timer.start();
}

void CreateDownloadDialog::onTimeOut()
{
    QNetworkRequest request;
    request.setUrl(m_url);
    request.setRawHeader("Range", "bytes=0-0");
    QNetworkReply *reply = m_manager.head(request);
    connect(reply, &QNetworkReply::finished, this, &CreateDownloadDialog::onHttpReplyFinished);
}

void CreateDownloadDialog::onHttpReplyFinished()
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }

    if (m_url != reply->url()) {
        return;
    }

    QRegExp exp("^(.*)/(\\d*)");
    QString range = reply->rawHeader("Content-Range");
    qint64 size = 0;
    if (exp.indexIn(range) != -1) {
        size = exp.cap(2).toLongLong();
    }

    QFileInfo file;
    file.setFile(m_url.path());

    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->tableView->model());
    model->setItem(0, 0, new QStandardItem(file.fileName()));
    model->setItem(0, 1, new QStandardItem(file.suffix()));
    model->setItem(0, 2, new QStandardItem(Utils::instance()->trasformUnit(size)));
}

void CreateDownloadDialog::on_btnPath_clicked()
{
    m_path = QFileDialog::getExistingDirectory(this,
        "选择路径",
        m_path,
        QFileDialog::ShowDirsOnly);

    if (!m_path.isEmpty()) {
        setDirectoryInfo(m_path);
    }
}

void CreateDownloadDialog::on_btnDownload_clicked()
{
    accept();
}
