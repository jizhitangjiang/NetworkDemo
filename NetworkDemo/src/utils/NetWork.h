#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QUrl>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQueue>
#include "FileHandler.h"

struct ReqData
{
    QUrl    m_url;
    int     m_rid = 0;
    FileHandler *m_fileHandler = NULL;
    QNetworkReply *m_reply = NULL;
};

class NetWork : public QObject
{
    Q_OBJECT

public:
    explicit NetWork(QObject *parent = 0);
    ~NetWork();

public:
    /**
     * @brief 下载文件
     * @param url 文件url
     * @param filePath 文件保存路径
     * @return 返回下载id
     */
    int downloadFile(const QString &url, const QString &filePath);
    /**
     * @brief stop 停止下载
     * @param rid 下载的id
     */
    void stop(int rid);

private:
    int enqueue(const QString &url, const QString &filePath);
    ReqData* dequeue();
    void deleteRequest(int rid);

signals:
    void downloadProgress(int rid, qint64 bytesReceived, qint64 bytesTotal);
   // void readReady(int rid, const QByteArray &data);
    void requestError(int rid, const QString &error);
    void requestFinished(int rid);

private slots:
    void onDelyRequest();
    void onHttpReplyFinished();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onReadReady();
    void onError(QNetworkReply::NetworkError code);

private:
    QNetworkAccessManager  m_manager;
    QQueue<ReqData*>       m_ready;
    QMutex                 m_mutex;
    int                    m_reqId;
    QMap<int, ReqData*>    m_running;
};

#endif // NETWORK_H
