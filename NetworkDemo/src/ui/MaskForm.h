#ifndef MASKFORM_H
#define MASKFORM_H

#include <QWidget>
#include <QApplication>

class MaskForm : public QWidget
{
    Q_OBJECT
public:
    explicit MaskForm(QWidget *topWidget = QApplication::activeWindow(),
        QWidget *parent = 0);
    ~MaskForm();
    /**
     * @brief 注册要显示在遮罩中的窗口
     * @param widget 要显示的窗口
     */
    void installWidget(QWidget *widget);
    /**
     * @brief 设置遮罩的颜色，透明度
     * @param color 遮罩颜色
     * @param opacity 透明度
     */
    void setMaskColor(const QColor &color, float opacity);

private:
    void init();
    QPoint globalPoint();
    void showEvent(QShowEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:

private:
    QWidget *m_topWidget;   //顶层窗口，设置遮罩大小
    QWidget *m_widget;      //遮罩中显示的窗口
};

#endif // MASKFORM_H
