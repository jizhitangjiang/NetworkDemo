#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include "NetWork.h"

class Utils : public QObject
{
    Q_OBJECT

private:
    class AutoDelete
    {
    private:
        AutoDelete(){}
        ~AutoDelete(){
            if (Utils::m_instance!= NULL) {
                delete Utils::m_instance;
                Utils::m_instance = NULL;
            }
        }
        static AutoDelete m_instance;
    };

private:
    explicit Utils(QObject *parent = 0);
    ~Utils();

public:
    /**
     * @brief 获得工具单例对象
     * @return 返回对象
     */
    static Utils *instance();
    /**
     * @brief 获得应用的CSS样式
     * @return 返回应用的CSS样式
     */
    QByteArray getAppCss();
    /**
     * @brief 获得网路模块
     * @return 返回网络模块对象
     */
    NetWork *network();
    QString trasformUnit(qint64 bytes);

private:
    static Utils *m_instance;
    NetWork *m_network;
};

#endif // UTILS_H
