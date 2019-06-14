#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>

/**
 * @brief The SystemTrayIcon class
 * @note 设置系统托盘
 */
class SystemTrayIcon : public QObject
{
    Q_OBJECT

public:
    enum TrayEventType {
        TrayEventType_Null,
        TrayEventType_Open,
        TrayEventType_Minimun,
        TrayEventType_Close,
        TrayEventType_DClick
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
    void onActivated(QSystemTrayIcon::ActivationReason reason);

private:
    QSystemTrayIcon m_trayIcon;
};

#endif // SYSTEMTRAYICON_H
