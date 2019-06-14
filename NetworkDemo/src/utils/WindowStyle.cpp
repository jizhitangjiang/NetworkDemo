#include "WindowStyle.h"

#include <QWidget>
#include <QDebug>

#define BORDER 30

CursorStyle::CursorStyle(QObject *parent)
    : QObject(parent)
    , m_type(ShapeType_Oright)
{

}

CursorStyle::~CursorStyle()
{

}

void CursorStyle::updateCursorPos(const QPoint &point, const QRect &rect)
{
    int cursorX = point.x();
    int cursorY = point.y();

    int widgetX = rect.x();
    int widgetY = rect.y();

    int widgetW = rect.width();
    int widgetH = rect.height();

    bool isLeftEdge = cursorX >= widgetX && cursorX <= widgetX + BORDER;
    bool isRightEdge = cursorX <= widgetX + widgetW && cursorX >= widgetX + widgetW - BORDER;
    bool isTopEdge = cursorY >= widgetY && widgetY <= widgetY + BORDER;
    bool isBottomEdge = cursorY <= widgetY + widgetH && cursorY >= widgetY + widgetH - BORDER;

    bool isTopLeftEdge = isTopEdge && isLeftEdge;
    bool isBottomLeftEdge = isBottomEdge && isLeftEdge;
    bool isTopRightEdge = isTopEdge && isRightEdge;
    bool isBottomRightEdge = isBottomEdge && isRightEdge;

    if (isTopLeftEdge) {
        m_type = ShapeType_Topleft;
    } else if (isBottomLeftEdge) {
        m_type = ShapeType_Bottomleft;
    } else if (isTopRightEdge) {
        m_type = ShapeType_Topright;
    } else if (isBottomRightEdge) {
        m_type = ShapeType_Bottomright;
    } else if (isLeftEdge) {
        m_type = ShapeType_Left;
    } else if (isRightEdge) {
        m_type = ShapeType_Right;
    } else if (isTopEdge) {
        m_type = ShapeType_Top;
    } else if (isBottomEdge) {
        m_type = ShapeType_Bottom;
    } else {
        m_type = ShapeType_Oright;
    }
}

void CursorStyle::updateCursorStyle(const QPoint &gPoint, const QRect &rect, QWidget *widget)
{
    updateCursorPos(gPoint, rect);

    if (m_type == ShapeType_Topleft || m_type == ShapeType_Bottomright) {
        widget->setCursor(Qt::SizeFDiagCursor);
    } else if (m_type == ShapeType_Topright || m_type == ShapeType_Bottomleft) {
        widget->setCursor(Qt::SizeBDiagCursor);
    } else if (m_type == ShapeType_Left || m_type == ShapeType_Right) {
        widget->setCursor(Qt::SizeHorCursor);
    } else if (m_type == ShapeType_Top || m_type == ShapeType_Bottom) {
        widget->setCursor(Qt::SizeVerCursor);
    } else {
         widget->unsetCursor();
    }
}

WindowStyle::WindowStyle(QObject *parent)
    : QObject(parent)
    , m_widget(NULL)
    , m_leftPressed(false)
{ 
}

WindowStyle::~WindowStyle()
{

}

void WindowStyle::activateOn(QWidget *widget)
{
    m_widget = widget;
    m_wnd = widget->window();

    m_widget->installEventFilter(this);
}

bool WindowStyle::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_widget) {
        QEvent::Type type = event->type();

        switch(type) {
        case QEvent::MouseButtonPress:
            mousePressHandle(m_widget, dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonRelease:
            mouseReleaseHandle(m_widget, dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseMove:
            mouseMoveHandle(m_widget, dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::MouseButtonDblClick:
            mouseDBClickedHandle(m_widget, dynamic_cast<QMouseEvent*>(event));
            break;
        case QEvent::HoverMove:
            mouseHoverHandle(m_widget, dynamic_cast<QMouseEvent*>(event));
            break;
        default:
            break;
        }
    }

    return QObject::eventFilter(watched, event);
}

void WindowStyle::mousePressHandle(QWidget *watched, QMouseEvent *event)
{
    Q_UNUSED(watched);

    if (event->button() == Qt::LeftButton) {
        m_leftPressed = true;
        m_distance = event->globalPos() - m_wnd->mapToGlobal(QPoint(0,0));
    }
}

void WindowStyle::mouseReleaseHandle(QWidget *watched, QMouseEvent *event)
{
    Q_UNUSED(watched);

    if (event->button() == Qt::LeftButton) {
        m_leftPressed = false;
    }
}

void WindowStyle::mouseMoveHandle(QWidget *watched, QMouseEvent *event)
{
    Q_UNUSED(watched);

    if (m_leftPressed) {
        m_wnd->move(event->globalPos() - m_distance);
    }
}

void WindowStyle::mouseDBClickedHandle(QWidget *watched, QMouseEvent *event)
{
    Q_UNUSED(watched);

    if (event->button() == Qt::LeftButton) {
        if (m_wnd->isMaximized()) {
            m_wnd->showNormal();
        } else {
            m_wnd->showMaximized();
        }
    }
}

void WindowStyle::mouseHoverHandle(QWidget *watched, QMouseEvent *event)
{
    m_cursorStyle.updateCursorStyle(event->globalPos(), m_wnd->geometry(), m_wnd);
}
