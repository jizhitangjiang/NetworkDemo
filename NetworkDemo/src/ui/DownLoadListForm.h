#ifndef DOWNLOADLISTFORM_H
#define DOWNLOADLISTFORM_H

#include <QWidget>
#include "DownloadItemForm.h"

namespace Ui {
class DownLoadListForm;
}

class DownLoadListForm : public QWidget
{
    Q_OBJECT

public:
    explicit DownLoadListForm(QWidget *parent = 0);
    ~DownLoadListForm();

    void createDownItemForm(const QString &url, const QString &filePath);

private slots:
    void on_btnCreate_clicked();

private:
    void init();

private:
    Ui::DownLoadListForm *ui;
    QList<DownloadItemForm *> m_downItemList;
};

#endif // DOWNLOADLISTFORM_H
