#ifndef SCALABELFRAMELESSWIDGET_H
#define SCALABELFRAMELESSWIDGET_H

#include <QWidget>
#include <QList>
class QMouseEvent;
class QResizeEvent;
class QPaintEvent;

//一个可伸缩的，无边框的面板类。
class ScalableFramelessWidget : public QWidget
{
    Q_OBJECT
public:
    enum MousePosition
    {
        HLeft = 1,
        HRight = 2,
        VTop = 3,
        VBottom = 4,
        TopLeft = 5,
        TopRight = 6,
        BottomLeft = 7,
        BottomRight = 8,
        Normal = 0
    };
public:
    ScalableFramelessWidget(QWidget* parent = 0);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void resizeEvent(QResizeEvent* e);
    void paintEvent(QPaintEvent *e);
    bool eventFilter(QObject *, QEvent *);
    void setHeaderHeight(int height);
    int headerHeight() const;
    //应用程序是否处于全屏状态，由子类对象进行设置
    void setScreenFull(bool isFull = true);
    QSize minimumSizeHint() const;
    void setIGeometry(QRect &);
private:
    void setICursor(MousePosition mPosition);
    //确定鼠标的区域，用于显示不同的鼠标图按。
    MousePosition getMousePosition(const QPoint& point);
    void updateArea();
    bool pointInRect(const QPoint &point, const QRect &rect);

private:
	int border_width_;
    int _headerHeight;
    bool _isLeftButtonPressed;
    bool _isFull;
    bool _isResizing;
    MousePosition _mPosition;
    QList<QRect> _rectList;//每次窗口调整大小完成时更新这些值
    QRect _currRect;//当前位置矩形
    QPoint _mPoint;
    QPoint _cPoint;//点击点，鼠标移动后的前一个点。

signals:
    void cancelClip();
};

#endif // SCALABELFRAMELESSWIDGET_H
