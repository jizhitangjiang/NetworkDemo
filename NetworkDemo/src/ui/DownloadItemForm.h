#ifndef DOWNLOADITEMFORM_H
#define DOWNLOADITEMFORM_H

#include <QWidget>
#include "NetWork.h"

namespace Ui {
class DownloadItemForm;
}

class DownloadItemForm : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadItemForm(QWidget *parent = 0);
    ~DownloadItemForm();

private:
    void init();
    QString trasformUnit(qint64 bytes);
    qint64 downloadSpeed(qint64 receiveBytes);
    QString remainTime(qint64 remainBytes, qint64 speed);

private slots:
    void onDownloadProgress(int rid, qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished(int rid, qint64 fileSize);
    void on_btnStop_clicked(bool checked);

private:
    Ui::DownloadItemForm *ui;
    NetWork *m_network;
    int      m_rid;
    QTime    m_time;
    qint64   m_recevieBytes;
};

#endif // DOWNLOADITEMFORM_H
