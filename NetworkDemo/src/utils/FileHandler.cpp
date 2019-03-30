#include "FileHandler.h"

#include <QDebug>

FileHandler::FileHandler(const QString &filePath, QObject *parent)
    : QObject(parent)
    , m_filePath(filePath)
{

}

FileHandler::~FileHandler()
{
    if (m_file.isOpen()) {
        m_file.close();
    }
}

bool FileHandler::openFile(qint64 &size)
{
    if (m_filePath.isEmpty()) {
        return false;
    }
    //同名文件存在删除
    if (QFile::exists(m_filePath)) {
        if (!QFile::remove(m_filePath)) {
            return false;
        }
    }

    QString tempPath = m_filePath + ".cxd";
    m_file.setFileName(tempPath);   
    size = m_file.size();

    return m_file.open(QIODevice::WriteOnly | QIODevice::Append);
}

bool FileHandler::writeFile(const QByteArray &data)
{
    if (data.isEmpty() || !m_file.isOpen()) {
        return false;
    }

    m_file.write(data);

    return true;
}

void FileHandler::closeFile()
{
    if (m_file.isOpen()) {
        m_file.close();
    }
}

void FileHandler::finishFile()
{
    closeFile();

    if (QFile::exists(m_filePath)) {
        if (!QFile::remove(m_filePath)) {
            return;
        }
    }

    m_file.rename(m_filePath);
}
