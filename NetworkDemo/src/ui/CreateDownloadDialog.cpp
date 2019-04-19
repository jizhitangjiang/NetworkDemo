#include "CreateDownloadDialog.h"
#include "ui_CreateDownloadDialog.h"

#include <QDebug>

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

void CreateDownloadDialog::init()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
}

void CreateDownloadDialog::showEvent(QShowEvent *event)
{
    qDebug()<<"QShowEvent"<<this->geometry();
}

void CreateDownloadDialog::on_btnClose_clicked()
{
    close();
}
