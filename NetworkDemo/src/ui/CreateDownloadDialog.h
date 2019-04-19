#ifndef CREATEDOWNLOADDIALOG_H
#define CREATEDOWNLOADDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDownloadDialog;
}

class CreateDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDownloadDialog(QWidget *parent = 0);
    ~CreateDownloadDialog();

private:
    void init();
    void showEvent(QShowEvent *event);

private slots:
    void on_btnClose_clicked();

private:
    Ui::CreateDownloadDialog *ui;
};

#endif // CREATEDOWNLOADDIALOG_H
