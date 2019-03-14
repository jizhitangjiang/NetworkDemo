#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>

class SystemTrayIcon : public QObject
{
    Q_OBJECT

public:
    enum TrayEventType {
        TrayEventType_Open,
        TrayEventType_Minimun,
        TrayEventType_Close
    };

public:
    explicit SystemTrayIcon(QObject *parent = 0);
    ~SystemTrayIcon();

private:
    void init();

signals:
    void trayEventActivate(TrayEventType type);

private slots:
    void onActionClicked(QAction *action);

private:
    QSystemTrayIcon m_trayIcon;
};

#endif // SYSTEMTRAYICON_H
