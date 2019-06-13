#include "NetWork.h"

#include <QTimer>
#include <QMutexLocker>
#include <QNetworkRequest>
#include <QRegExp>

static const int s_requestSize = 20;

NetWork::NetWork(QObject *parent)
    : QObject(parent)
    , m_reqId(0)
{
}

NetWork::~NetWork()
{
    QList<int> keys = m_running.keys();
    for (auto &key : keys) {
        deleteRequest(key);
    }
}

int NetWork::downloadFile(const QString &url, const QString &filePath)
{
    if (url.isEmpty() || filePath.isEmpty()) {
        return -1;
    }

    return enqueue(url, filePath);
}

void NetWork::stop(int rid)
{
    if (!m_running.contains(rid)) {
        return;
    }

    ReqData *reqData = m_running.value(rid, nullptr);
    if (reqData == nullptr) {
        return;
    }

    QNetworkReply *reply = reqData->m_reply;
    if (reply != nullptr) {
        disconnect(reply, &QNetworkReply::finished, this,
            &NetWork::onHttpReplyFinished);
        disconnect(reply, &QNetworkReply::downloadProgress, this,
            &NetWork::onDownloadProgress);
        disconnect(reply, &QNetworkReply::readyRead, this,
            &NetWork::onReadReady);
        disconnect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
                this, &NetWork::onError);
       reply->abort();
    }

    if (reqData->m_fileHandler != nullptr) {
        reqData->m_fileHandler->closeFile();
    }

    deleteRequest(rid);
}

int NetWork::enqueue(const QString &url, const QString &filePath)
{
    QMutexLocker locker(&m_mutex);

    ReqData *req = new ReqData;
    req->m_rid = ++m_reqId;
    req->m_url = QUrl(url);
    req->m_fileHandler = new FileHandler(filePath, this);

    m_ready.enqueue(req);
    QTimer::singleShot(0, this, &NetWork::onDelyRequest);

    return m_reqId;
}

ReqData* NetWork::dequeue()
{
    QMutexLocker locker(&m_mutex);
    if (m_ready.isEmpty()) {
        return nullptr;
    }

    return m_ready.dequeue();
}

void NetWork::deleteRequest(int rid)
{
    if (!m_running.contains(rid)) {
        return;
    }

    ReqData *reqData = m_running.value(rid, nullptr);
    if (reqData != nullptr) {
        if (reqData->m_reply != nullptr) {
            reqData->m_reply->deleteLater();
            reqData->m_reply = nullptr;
        }

        if (reqData->m_fileHandler != nullptr) {
            reqData->m_fileHandler->deleteLater();
            reqData->m_fileHandler = nullptr;
        }
    }

    m_running.remove(rid);
}

void NetWork::onDelyRequest()
{
    if (m_running.size() >= s_requestSize) {
        QTimer::singleShot(10, this, &NetWork::onDelyRequest);
        return;
    }

    ReqData* reqData = dequeue();
    if (reqData == nullptr) {
        return;
    }

    qint64 size = 0;
    if (!reqData->m_fileHandler->openFile(size)) {
        emit requestError(reqData->m_rid, "文件打开失败");
        deleteRequest(reqData->m_rid);
        return;
    }

    QNetworkRequest req(reqData->m_url);
    if(size > 0) {
        QByteArray recvByte = QString("bytes=%1-").arg(size).toUtf8();
        req.setRawHeader("Range", recvByte);
    }

    QNetworkReply *reply = m_manager.get(req);
    if (reply != nullptr) {
        reqData->m_reply = reply;
        reply->setProperty("rid", reqData->m_rid);

        connect(reply, &QNetworkReply::finished, this,
            &NetWork::onHttpReplyFinished);
        connect(reply, &QNetworkReply::downloadProgress, this,
            &NetWork::onDownloadProgress);
        connect(reply, &QNetworkReply::readyRead, this,
            &NetWork::onReadReady);
        connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
                this, &NetWork::onError);

        m_running.insert(reqData->m_rid, reqData);
    }

    QTimer::singleShot(10, this, &NetWork::onDelyRequest);
    return;
}

void NetWork::onHttpReplyFinished()
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }

    int rid = reply->property("rid").toInt();
    ReqData *reqData = m_running.value(rid, nullptr);
    if (reqData != nullptr) {
        reqData->m_fileHandler->renameFile();
        if (reply->error() == QNetworkReply::NoError && rid > 0) {
            emit requestFinished(rid, reqData->m_fileHandler->fileSize());
        }
    }

    deleteRequest(rid);
}

void NetWork::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }

    //从http响应头中获得已经下载的文件大小
    QRegExp exp("bytes\\s(\\d*)");
    QString range = reply->rawHeader("Content-Range");
    qint64 size = 0;
    if (exp.indexIn(range) != -1) {
       size = exp.cap(1).toLongLong();
    }

    int rid = reply->property("rid").toInt();
    if (rid > 0) {
        emit downloadProgress(rid, bytesReceived + size, bytesTotal + size);
    }
}

void NetWork::onReadReady()
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }
    int rid = reply->property("rid").toInt();
    ReqData *reqData = m_running.value(rid, nullptr);
    if (reqData != nullptr) {
        if (!reqData->m_fileHandler->writeFile(reply->readAll())) {
            stop(rid);
            emit requestError(rid, "文件写入失败");
        }
    }
}

void NetWork::onError(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code);
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if (reply == nullptr) {
        return;
    }

    int rid = reply->property("rid").toInt();
    if (rid > 0) {
        QString error = reply->errorString();
        emit requestError(rid, error);
    }

}
