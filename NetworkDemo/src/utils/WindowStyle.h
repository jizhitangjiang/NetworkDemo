#ifndef WINDOWSTYLE_H
#define WINDOWSTYLE_H

#include <QObject>
#include <QMouseEvent>

/**
 * @brief The WindowStyle class
 * @note 将窗口注册在此，进行拖拽移动，双击放大缩小
 */
class WindowStyle : public QObject
{
    Q_OBJECT
public:
    explicit WindowStyle(QObject *parent = 0);
    ~WindowStyle();
    /**
     * @brief 窗口注册
     * @param 注册窗口
     */
    void activateOn(QWidget *widget);

private:
    bool eventFilter(QObject *watched, QEvent *event);
    void mousePressHandle(QWidget *watched, QMouseEvent *event);
    void mouseMoveHandle(QWidget *watched, QMouseEvent *event);
    void mouseDBClicked(QWidget *watched, QMouseEvent *event);

private:
    QPoint m_distance;
    QWidget *m_widget;
};

#endif // WINDOWSTYLE_H
