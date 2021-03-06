﻿#include "NetWork.h"

#include <QTimer>
#include <QMutexLocker>
#include <QNetworkRequest>

static const int s_requestSize = 10;

NetWork::NetWork(QObject *parent)
    : QObject(parent)
    , m_reqId(0)
{
}

NetWork::~NetWork()
{
    for (const auto &item : m_running) {
        deleteRequest(item->m_rid);
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

    ReqData *reqData = m_running.value(rid, NULL);
    if (reqData == NULL) {
        return;
    }

    QNetworkReply *reply = reqData->m_reply;
    if (reply!= NULL) {
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

    if (reqData->m_fileHandler != NULL) {
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
        return NULL;
    }

    return m_ready.dequeue();
}

void NetWork::deleteRequest(int rid)
{
    if (!m_running.contains(rid)) {
        return;
    }

    ReqData *reqData = m_running.value(rid, NULL);
    if (reqData != NULL) {
        if (reqData->m_reply != NULL) {
            reqData->m_reply->deleteLater();
            reqData->m_reply = NULL;
        }

        if (reqData->m_fileHandler != NULL) {
            reqData->m_fileHandler->deleteLater();
            reqData->m_fileHandler = NULL;
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
    if (reqData == NULL) {
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
    if (reply != NULL) {
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
    if (reply == NULL) {
        return;
    }

    int rid = reply->property("rid").toInt();
    ReqData *reqData = m_running.value(rid, NULL);
    if (reqData != NULL) {
        reqData->m_fileHandler->finishFile();
        if (reply->error() == QNetworkReply::NoError && rid > 0) {
            emit requestFinished(rid);
        }
    }

    deleteRequest(rid);
}

void NetWork::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if (reply == NULL) {
        return;
    }
    QString range = reply->rawHeader("Content-Range");
    qint64 size = range.left(range.indexOf("-")).toLongLong();
    int rid = reply->property("rid").toInt();
    if (rid > 0) {
        emit downloadProgress(rid, bytesReceived + size, bytesTotal + size);
    }
}

void NetWork::onReadReady()
{
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(sender());
    if (reply == NULL) {
        return;
    }
    int rid = reply->property("rid").toInt();
    ReqData *reqData = m_running.value(rid, NULL);
    if (reqData != NULL) {
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
    if (reply == NULL) {
        return;
    }

    int rid = reply->property("rid").toInt();
    if (rid > 0) {
        QString error = reply->errorString();
        emit requestError(rid, error);
    }

}
