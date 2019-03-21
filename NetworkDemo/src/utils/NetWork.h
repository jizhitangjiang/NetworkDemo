#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQueue>

struct ReqData
{
    QUrl    m_url = "";
    int     m_rid = 0;
    QString m_filePath = "";
    QNetworkReply *m_reply = NULL;
};

class NetWork : public QObject
{
    Q_OBJECT

public:
    explicit NetWork(QObject *parent = 0);
    ~NetWork();

public:
    int downloadFile(const QString &url, const QString &filePath);
    void stop(int rid);

private:
    int enqueue(const QString &url, const QString &filePath);
    ReqData* dequeue();
    void deleteRequest(int rid);

signals:
    void downloadProgress(int rid, qint64 bytesReceived, qint64 bytesTotal);
    void readReady(int rid, const QByteArray &data);
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
