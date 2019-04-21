#include "Utils.h"

#include <QDir>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QDebug>

#define SIZE_KB 1024
#define SIZE_MB 1024 * 1024
#define SIZE_GB 1024 * 1024 *1024

Utils* Utils::m_instance = nullptr;
Utils::AutoDelete Utils::AutoDelete::m_instance;

Utils::Utils(QObject *parent)
    : QObject(parent)
    , m_network(nullptr)
{

}

Utils::~Utils()
{
}

Utils* Utils::instance()
{
    if (m_instance == nullptr) {
        m_instance = new Utils;
    }

    return m_instance;
}

QByteArray Utils::getAppCss()
{
    QDir dir("://css/");
    QStringList filters;
    filters << "*.css" << "*.qss";

    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    QByteArray css;
    QFileInfoList list = dir.entryInfoList();
    for (const auto &fileInfo : list) {
        QFile file(fileInfo.filePath());
        if (file.open(QIODevice::ReadOnly)) {
            css.append(file.readAll());
            file.close();
        }
    }
    qDebug()<<css;
    return css;
}

NetWork *Utils::network()
{
    if (m_network == nullptr) {
        m_network = new NetWork(this);
    }

    return m_network;
}


QString Utils::trasformUnit(qint64 bytes)
{
    QString unit = "B";
    if (bytes < SIZE_KB) {

    } else if (bytes < SIZE_MB) {
        bytes /= SIZE_KB;
        unit = "KB";
    } else if (bytes < SIZE_GB) {
        bytes /= SIZE_MB;
        unit = "MB";
    } else {
        bytes /= SIZE_GB;
        unit = "GB";
    }

    return  QString("%1%2").arg(bytes).arg(unit);
}

