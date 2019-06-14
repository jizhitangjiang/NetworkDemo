#ifndef MASKFORM_H
#define MASKFORM_H

#include <QWidget>
#include <QApplication>

class MaskForm : public QWidget
{
    Q_OBJECT
public:
    explicit MaskForm(QWidget *parent = 0);
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

    /**
     * @brief 设置顶层窗口(覆盖窗口)
     * @param widget 顶层窗口
     */
    void setTopWidget(QWidget *widget);

private:
    void init();
    void showEvent(QShowEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QWidget *m_topWidget;   //顶层窗口，设置遮罩大小
    QWidget *m_widget;      //遮罩中显示的窗口
};

#endif // MASKFORM_H
