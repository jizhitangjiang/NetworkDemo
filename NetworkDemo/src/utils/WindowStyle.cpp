#include "WindowStyle.h"

#include <QWidget>
#include <QDebug>

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
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);

        switch(type) {
        case QEvent::MouseButtonPress:
            mousePressHandle(m_widget, mouseEvent);
            break;
        case QEvent::MouseButtonRelease:
            mouseReleaseHandle(m_widget, mouseEvent);
            break;
        case QEvent::MouseMove:
            mouseMoveHandle(m_widget, mouseEvent);
            break;
        case QEvent::MouseButtonDblClick:
            mouseDBClicked(m_widget, mouseEvent);
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

void WindowStyle::mouseDBClicked(QWidget *watched, QMouseEvent *event)
{
    Q_UNUSED(watched);

    if (event->button() != Qt::LeftButton) {
        return;
    }

    if (m_wnd->isMaximized()) {
        m_wnd->showNormal();
    } else {
        m_wnd->showMaximized();
    }
}
