#ifndef WINDOWSTYLE_H
#define WINDOWSTYLE_H

#include <QObject>
#include <QMouseEvent>

class CursorStyle : public QObject
{
public:
    enum ShapeType
    {
        ShapeType_Oright,
        ShapeType_Left,
        ShapeType_Right,
        ShapeType_Top,
        ShapeType_Bottom,
        ShapeType_Topleft,
        ShapeType_Bottomleft,
        ShapeType_Topright,
        ShapeType_Bottomright,
    };

public:
    explicit CursorStyle(QObject *parent = 0);
    ~CursorStyle();

    void updateCursorStyle(const QPoint &point, const QRect &rect, QWidget *widget);

private:
    void updateCursorPos(const QPoint &point, const QRect &rect);

private:
    ShapeType m_type;
};

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
    void mouseReleaseHandle(QWidget *watched, QMouseEvent *event);
    void mouseMoveHandle(QWidget *watched, QMouseEvent *event);
    void mouseDBClickedHandle(QWidget *watched, QMouseEvent *event);
    void mouseHoverHandle(QWidget *watched, QMouseEvent *event);

private:
    QPoint   m_distance;    //记录点击位置与m_wnd左上角距离
    QWidget *m_widget;      //鼠标可以操作的窗口
    QWidget *m_wnd;         //进行缩放，移动的窗口
    bool     m_leftPressed; //鼠标左键是否按下
    CursorStyle m_cursorStyle;
};

#endif // WINDOWSTYLE_H
