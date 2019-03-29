#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QFile>

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(const QString &filePath, QObject *parent = 0);
    ~FileHandler();

public:
    bool openFile(qint64 &size);
    bool writeFile(const QByteArray &data);
    void closeFile();

private:
    QFile   m_file;
    QString m_filePath;
};

#endif // FILEHANDLER_H
