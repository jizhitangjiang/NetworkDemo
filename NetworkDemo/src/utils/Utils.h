#ifndef UTILS_H
#define UTILS_H

#include <QString>

class Utils
{
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
    Utils();
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

private:
    static Utils *m_instance;
};

#endif // UTILS_H
