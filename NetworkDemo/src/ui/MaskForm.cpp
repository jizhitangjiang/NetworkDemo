#include "MaskForm.h"

#include <QDebug>
#include <QGridLayout>

MaskForm::MaskForm(QWidget *topWidget, QWidget *parent)
    : m_topWidget(topWidget)
    , m_widget(nullptr)
    , QWidget(parent)
{
    init();
}

MaskForm::~MaskForm()
{

}

void MaskForm::installWidget(QWidget *widget)
{
    if (widget == nullptr) {
        return;
    }

    widget->installEventFilter(this);
    m_widget = widget;

    connect(m_widget, &QWidget::destroyed, this, [=](){
        m_widget = nullptr;
        });
}

void MaskForm::setMaskColor(const QColor &color, float opacity)
{
    if (!color.isValid()) {
        return;
    }

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, color);
    this->setPalette(palette);

    setWindowOpacity(opacity);
}

void MaskForm::init()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setMaskColor(QColor(245, 125, 0), 0.6f);
}

QPoint MaskForm::globalPoint()
{
    QPoint point;
    point.setX((width() - m_widget->width()) / 2);
    point.setY((height() - m_widget->height()) / 2);

    return mapToGlobal(point);
}

void MaskForm::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    this->setGeometry(m_topWidget->geometry());
    m_widget->move(globalPoint());
}

bool MaskForm::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_widget) {
        if (event->type() == QEvent::Show) {
            this->show();
        }

        if (event->type() == QEvent::Hide) {
            this->hide();
        }
    }

    return QObject::eventFilter(watched, event);
}
