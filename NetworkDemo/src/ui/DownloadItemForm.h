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

private slots:
    void on_btnStart_clicked();
    void onDownloadProgress(int rid, qint64 bytesReceived, qint64 bytesTotal);

private:
    Ui::DownloadItemForm *ui;
    NetWork *m_network;
};

#endif // DOWNLOADITEMFORM_H
