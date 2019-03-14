#include "Utils.h"

#include <QDir>
#include <QSystemTrayIcon>
#include <QDebug>

Utils* Utils::m_instance = NULL;
Utils::AutoDelete Utils::AutoDelete::m_instance;

Utils::Utils()
{
}

Utils::~Utils()
{
}

Utils* Utils::instance()
{
    if (m_instance == NULL) {
        m_instance = new Utils;
    }
    return m_instance;
}

QByteArray Utils::getAppCss()
{
    QDir dir(":/res/css/");
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

    return css;
}


