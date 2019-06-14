#ifndef CREATEDOWNLOADDIALOG_H
#define CREATEDOWNLOADDIALOG_H

#include <QUrl>
#include <QDialog>
#include <QTimer>
#include <QNetworkAccessManager>

namespace Ui {
class CreateDownloadDialog;
}

class CreateDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDownloadDialog(QWidget *parent = 0);
    ~CreateDownloadDialog();

    void getDownloadInfo(QString &url, QString &path);

private:
    void init();
    qint64 getDiskFreeSize(QString path);
    void setDirectoryInfo(const QString &dir);

private slots:
    void on_btnClose_clicked();
    void on_textEdit_textChanged();
    void onTimeOut();
    void onHttpReplyFinished();
    void on_btnPath_clicked();

    void on_btnDownload_clicked();

private:
    Ui::CreateDownloadDialog *ui;
    QTimer                m_timer;
    QUrl                  m_url;
    QNetworkAccessManager m_manager;
    QString               m_path;
};

#endif // CREATEDOWNLOADDIALOG_H
