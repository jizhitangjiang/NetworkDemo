#include "WindowStyle.h"

#include <QWidget>
#include <QDebug>

WindowStyle::WindowStyle(QObject *parent)
    : QObject(parent)
    , m_widget(NULL)
{

}

WindowStyle::~WindowStyle()
{

}

void WindowStyle::activateOn(QWidget *widget)
{
    m_widget = widget;
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
    m_distance = event->globalPos() - watched->geometry().topLeft();
}

void WindowStyle::mouseMoveHandle(QWidget *watched, QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        watched->move(event->globalPos() - m_distance);
    }
}

void WindowStyle::mouseDBClicked(QWidget *watched, QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton) {
        return;
    }

    if (watched->isMaximized()) {
        watched->showNormal();
    } else {
        watched->showMaximized();
    }
}
