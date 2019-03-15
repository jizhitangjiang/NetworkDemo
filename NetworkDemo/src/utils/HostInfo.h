#ifndef HOSTINFO_H
#define HOSTINFO_H

#include <QHostInfo>
#include <QObject>

class HostInfo : public QObject
{
    Q_OBJECT

public:
    HostInfo(QObject *parent = NULL);
    ~HostInfo();

public:
    /**
     * @brief 获得本地主机名
     * @return
     */
    QString hostName();
    /**
     * @brief 获得本地IP地址列表
     * @return
     */
    QString hostAddress();
    /**
     * @brief 根据主机名或ip地址，获得指定主机名或IP地址
     * @param domain 指定域名
     */
    void getDomainAddress(const QString &domain);

signals:
    void lookupHostFinish(bool isSucessed,
        const QJsonObject &obj,
        const QString &error);

private slots:
    void onLookupHostFinished(const QHostInfo &info);

private:
    QString protocolName(QAbstractSocket::NetworkLayerProtocol type);
};

#endif // HOSTINFO_H
