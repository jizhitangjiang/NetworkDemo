#include "DownLoadListForm.h"
#include "ui_DownLoadListForm.h"
#include "CreateDownloadDialog.h"
#include "MaskForm.h"

#include <QStandardItemModel>

DownLoadListForm::DownLoadListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownLoadListForm)
{
    ui->setupUi(this);

    init();
}

DownLoadListForm::~DownLoadListForm()
{
    delete ui;
}

void DownLoadListForm::createDownItemForm(const QString &url, const QString &filePath)
{
    if (url.isEmpty() || filePath.isEmpty()) {
        return;
    }

    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>
        (ui->tableView->model());

    if (model == nullptr) {
        return;
    }

    DownloadItemForm *form = new DownloadItemForm(url, filePath);

    int row = model->rowCount();
    model->setItem(row, new QStandardItem);
    ui->tableView->setIndexWidget(model->index(row, 0), form);
    ui->tableView->setRowHeight(row, form->height());

    m_downItemList.append(form);
}

void DownLoadListForm::init()
{
    QStandardItemModel *model = new QStandardItemModel;
    ui->tableView->setModel(model);

    //最后一列拉伸
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void DownLoadListForm::on_btnCreate_clicked()
{
    MaskForm maskForm;
    CreateDownloadDialog dlg(&maskForm);
    maskForm.installWidget(&dlg);

    if (dlg.exec() == QDialog::Accepted) {
        QString url;
        QString filePath;
        dlg.getDownloadInfo(url, filePath);
        createDownItemForm(url, filePath);
    }
}
