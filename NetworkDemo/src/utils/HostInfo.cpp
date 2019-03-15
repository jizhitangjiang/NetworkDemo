#include "HostInfo.h"

#include <QJsonObject>

HostInfo::HostInfo(QObject *parent)
    : QObject(parent)
{

}

HostInfo::~HostInfo()
{

}

QString HostInfo::hostName()
{
    return QHostInfo::localHostName() + "\n";
}

QString HostInfo::hostAddress()
{
    //根据主机名，同步查找
    QHostInfo host = QHostInfo::fromName(QHostInfo::localHostName());
    QList<QHostAddress> addressList = host.addresses();
    QString data;

    for (const auto &address : addressList) {
        if (address.isNull()) {
            continue;
        }
        QString item = protocolName(address.protocol()) + address.toString() + "\r\n";
        data.append(item);
    }

    return data;
}

void HostInfo::getDomainAddress(const QString &domain)
{
    //根据主机名异步查找
    QHostInfo::lookupHost(domain, this, SLOT(onLookupHostFinished(QHostInfo)));
}

void HostInfo::onLookupHostFinished(const QHostInfo &info)
{
    if (info.error() != QHostInfo::NoError) {
        emit lookupHostFinish(false, QJsonObject(), info.errorString());
        return;
    }

    QString ip;
    QList<QHostAddress> addressList = info.addresses();
    for (const auto &address : addressList) {
        if (address.isNull()) {
            continue;
        }
        QString item = protocolName(address.protocol()) + address.toString() + "\n";
        ip.append(item);
    }

    QJsonObject obj;
    obj.insert("host", QJsonValue(info.hostName()));
    obj.insert("ip", QJsonValue(ip));

    emit lookupHostFinish(true, obj, "");
}

QString HostInfo::protocolName(QAbstractSocket::NetworkLayerProtocol type)
{
    switch (type) {
    case QAbstractSocket::IPv4Protocol:
        return "IPv4Protocol:";
        break;
    case QAbstractSocket::IPv6Protocol:
        return "IPv6Protocol:";
        break;
    case QAbstractSocket::UnknownNetworkLayerProtocol:
        return "UnknownNetworkLayerProtocol:";
        break;
    }
    return "";
}
