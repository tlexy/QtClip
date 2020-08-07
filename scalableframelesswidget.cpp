#include "scalableframelesswidget.h"
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QDebug>
#include <QtMath>

ScalableFramelessWidget::ScalableFramelessWidget(QWidget* parent)
    :_headerHeight(46), _isResizing(true), _isLeftButtonPressed(false), _isFull(false), border_width_(1), QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    setAttribute(Qt::WA_TranslucentBackground);
    for (int i = 0; i < 9; ++i)
    {
        _rectList.push_back(QRect());
    }
    //installEventFilter(this);
    updateArea();
    setMouseTracking(true);
}
void ScalableFramelessWidget::setHeaderHeight(int height)
{
    _headerHeight = height;
}

int ScalableFramelessWidget::headerHeight() const
{
    return _headerHeight;
}

QSize ScalableFramelessWidget::minimumSizeHint() const
{
    return QSize(800 + 8*2, 590 + 8*2);
}

bool ScalableFramelessWidget::eventFilter(QObject *obj, QEvent *e)
{
    //qDebug()<<e;
    return false;
}

void ScalableFramelessWidget::paintEvent(QPaintEvent *e)
{
	int border = border_width_;
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(border, border, this->width() - 2 * border, this->height() - 2 * border);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));
	QColor color(79, 79, 79, 100);
	for (int i = 1; i <= border; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(border - i, border - i, this->width() - (border - i), this->height() - (border - i));
		painter.setPen(color);
		painter.drawPath(path);
	}
}

void ScalableFramelessWidget::resizeEvent(QResizeEvent *e)
{
    if (_isLeftButtonPressed)
    {
        _isResizing = true;
    }
    updateArea();
}

void ScalableFramelessWidget::mousePressEvent(QMouseEvent* e)
{
    //qDebug()<<"mouse press...";
    if (e->button() == Qt::LeftButton)
    {
        _isLeftButtonPressed = true;
        _cPoint = e->globalPos();//获得全局坐标

        _currRect.setX(_cPoint.x() - e->pos().x());
        _currRect.setY(_cPoint.y() - e->pos().y());
        _currRect.setWidth(width());
        _currRect.setHeight(height());
    }
    if (e->button() == Qt::RightButton)
    {
        emit cancelClip();
    }
    //qDebug()<<e->pos();
    QWidget::mousePressEvent(e);
}

void ScalableFramelessWidget::mouseReleaseEvent(QMouseEvent* e)
{
    QWidget::setCursor(Qt::ArrowCursor);
    if (e->button() == Qt::LeftButton)
    {
        _isLeftButtonPressed = false;
    }
    if (_isResizing)
    {
        //qDebug()<<"update area...";
        updateArea();//如果刚刚正在调整大小，那么在释放鼠标时就要更新鼠标点击的特殊区域。
        _isResizing = false;
    }
    //qDebug()<<e->pos();
    QWidget::mouseReleaseEvent(e);
}

void ScalableFramelessWidget::setScreenFull(bool isFull)
{
    _isFull = isFull;
    if (_isFull)
    {
        showFullScreen();
    }
    else
    {
        showNormal();
    }
}

//这个方法要继续改正和完善，因为在不同的调整大小动作下，其行为应该不一样的。
void ScalableFramelessWidget::setIGeometry(QRect &rect)
{
    if (rect.width() < minimumWidth())
    {
        if (_mPosition == MousePosition::HLeft)
        {
            rect.setX(rect.x() + rect.width() - minimumWidth());
        }
        else if (_mPosition == MousePosition::HRight)
        {
            rect.setWidth(minimumWidth());
        }
        else if (_mPosition == MousePosition::TopLeft)
        {
            //
            rect.setX(rect.x() + rect.width() - minimumWidth());
        }
        else if (_mPosition == MousePosition::TopRight)
        {
            rect.setTopRight(QPoint(rect.bottomLeft().x() + minimumWidth(), rect.topRight().y()));
        }
        else if (_mPosition == MousePosition::BottomLeft)
        {
            rect.setBottomLeft(QPoint(rect.topRight().x() - minimumWidth(), rect.bottomLeft().y()));
        }
        else if (_mPosition == MousePosition::BottomRight)
        {
            rect.setBottomRight(QPoint(rect.topLeft().x() + minimumWidth(), rect.bottomRight().y()));
        }
        else
        {
            //MousePosition::Normal
        }
        //_mPosition = MousePosition::Normal;
        //setICursor(_mPosition);
    }
    if (rect.height() < minimumHeight())
    {
        if (_mPosition == MousePosition::VTop)
        {
            rect.setY(rect.y() + rect.height() - minimumHeight());
        }
        else if (_mPosition == MousePosition::VBottom)
        {
            rect.setHeight(minimumHeight());
        }
        else if(_mPosition == MousePosition::TopLeft)
        {
            rect.setY(rect.y() + rect.height() - minimumHeight());
        }
        else if (_mPosition == MousePosition::TopRight)
        {
            rect.setTopRight(QPoint(rect.topRight().x(), rect.bottomLeft().y() - minimumHeight()));
        }
        else if (_mPosition == MousePosition::BottomLeft)
        {
            rect.setBottomLeft(QPoint(rect.bottomLeft().x(), rect.topRight().y() + minimumHeight()));
        }
        else if (_mPosition == MousePosition::BottomRight)
        {
            rect.setBottomRight(QPoint(rect.bottomRight().x(), rect.topLeft().y() + minimumHeight()));
        }
        else
        {
            //MousePosition::Normal
        }
        //_mPosition = MousePosition::Normal;
        //setICursor(_mPosition);
    }
    QWidget::setGeometry(rect);
}

void ScalableFramelessWidget::setICursor(MousePosition mPosition)
{
    switch(mPosition)
    {
    case MousePosition::HLeft:
        QWidget::setCursor(Qt::SizeHorCursor);
        break;
    case MousePosition::HRight:
        QWidget::setCursor(Qt::SizeHorCursor);
        break;
    case MousePosition::VTop:
        QWidget::setCursor(Qt::SizeVerCursor);
        break;
    case MousePosition::VBottom:
        QWidget::setCursor(Qt::SizeVerCursor);
        break;
    case MousePosition::TopLeft:
        QWidget::setCursor(Qt::SizeFDiagCursor);
        break;
    case MousePosition::TopRight:
        QWidget::setCursor(Qt::SizeBDiagCursor);
        break;
    case MousePosition::BottomLeft:
        QWidget::setCursor(Qt::SizeBDiagCursor);
        break;
    case MousePosition::BottomRight:
        QWidget::setCursor(Qt::SizeFDiagCursor);
        break;
    default:
        QWidget::setCursor(Qt::ArrowCursor);
        break;
    }
}

bool ScalableFramelessWidget::pointInRect(const QPoint &point, const QRect &rect)
{
    if (point.x() > rect.x()
            && point.y() > rect.y()
            && rect.x() + rect.width() > point.x()
            && rect.y() + rect.height() > point.y()
            )
    {
        return true;
    }
    return false;
}

//修改9个区域的区域矩形（在构造函数中初始化）
void ScalableFramelessWidget::updateArea()
{
    //position 0
    _rectList[0].setRect(5, 5, width() - 5 - 5, height() - 5 - 5);
    //position 1
    _rectList[1].setRect(0,0,5,5);//只有这个值是不变的。
    //position 2
    _rectList[2].setRect(5, 0, width() - 5 - 5, 5);
    //position 3
    _rectList[3].setRect(width() - 5, 0, 5, 5);
    //position 4
    _rectList[4].setRect(0, 5, 5, height() - 5 - 5);
    //position 5
    _rectList[5].setRect(width() - 5, 5, 5, height() - 5 - 5);
    //position 6
    _rectList[6].setRect(0, height() - 5, 5, 5);
    //position 7
    _rectList[7].setRect(5, height() - 5, width() - 5 - 5, 5);
    //position 8
    _rectList[8].setRect(width() - 5, height() - 5, 5, 5);
}

ScalableFramelessWidget::MousePosition ScalableFramelessWidget::getMousePosition(const QPoint &point)
{
    if (pointInRect(point, _rectList[0]))
    {
        return MousePosition::Normal;//首先对这个大的区域进行检查。。。
    }
    else if (pointInRect(point, _rectList[1]))
    {
        return MousePosition::TopLeft;
    }
    else if (pointInRect(point, _rectList[2]))
    {
        return MousePosition::VTop;
    }
    else if (pointInRect(point, _rectList[3]))
    {
        return MousePosition::TopRight;
    }
    else if (pointInRect(point, _rectList[4]))
    {
        return MousePosition::HLeft;
    }
    else if (pointInRect(point, _rectList[5]))
    {
        return MousePosition::HRight;
    }
    else if (pointInRect(point, _rectList[6]))
    {
        return MousePosition::BottomLeft;
    }
    else if (pointInRect(point, _rectList[7]))
    {
        return MousePosition::VBottom;
    }
    else if (pointInRect(point, _rectList[8]))
    {
        return MousePosition::BottomRight;
    }
    else
    {
        //正常的程序不应该走到这一步
        return MousePosition::Normal;
    }
}

void ScalableFramelessWidget::mouseMoveEvent(QMouseEvent* e)
{
    //qDebug()<<"mouse move2..."<<"-pressed:"<<_isLeftButtonPressed;
    if (!_isLeftButtonPressed)
    {
        _mPosition = getMousePosition(e->pos());
        setICursor(_mPosition);
    }
    //qDebug()<<_mPosition;
    if (_isLeftButtonPressed && _mPosition != MousePosition::Normal)
    {
        _mPoint = e->globalPos();
        int x, y;
        switch(_mPosition)
        {
        case MousePosition::HLeft:
            x = _mPoint.x() - _cPoint.x();
            if ((_currRect.width() <= minimumWidth() && x > 0))
            {
                _mPosition = MousePosition::Normal;
                setICursor(_mPosition);
                break;
            }
            else if (x== 0)
            {
                break;
            }
            _currRect.setX(_currRect.x() + x);
            setIGeometry(_currRect);
            _cPoint.setX(_mPoint.x());
            _cPoint.setY(_mPoint.y());
            break;
        case MousePosition::HRight:
            x = _mPoint.x() - _cPoint.x();
            if ((_currRect.width() <= minimumWidth() && x < 0))
            {
                _mPosition = MousePosition::Normal;
                setICursor(_mPosition);
                break;
            }
            else if (x== 0)
            {
                break;
            }
            _currRect.setWidth(_currRect.width() + x);
            setIGeometry(_currRect);
            _cPoint.setX(_mPoint.x());
            _cPoint.setY(_mPoint.y());
            break;
        case MousePosition::VTop:
            y = _mPoint.y() - _cPoint.y();
            if ((_currRect.height() <= minimumHeight() && y > 0))
            {
                _mPosition = MousePosition::Normal;
                setICursor(_mPosition);
                break;
            }
            else if (y == 0)
            {
                break;
            }
            _currRect.setY(_currRect.y() + y);
            setIGeometry(_currRect);
            _cPoint.setX(_mPoint.x());
            _cPoint.setY(_mPoint.y());
            break;
        case MousePosition::VBottom:
            y = _mPoint.y() - _cPoint.y();
            if ((_currRect.height() <= minimumHeight() && y < 0))
            {
                _mPosition = MousePosition::Normal;
                setICursor(_mPosition);
                break;
            }
            else if (y == 0)
            {
                break;
            }
            _currRect.setHeight(_currRect.height() + y);
            setIGeometry(_currRect);
            _cPoint.setX(_mPoint.x());
            _cPoint.setY(_mPoint.y());
            break;
        case MousePosition::TopLeft:
            _currRect.setTopLeft(e->globalPos());
            setIGeometry(_currRect);
            break;
        case MousePosition::TopRight:
            _currRect.setTopRight(e->globalPos());
            setIGeometry(_currRect);
            break;
        case MousePosition::BottomLeft:
            _currRect.setBottomLeft(e->globalPos());
            setIGeometry(_currRect);
            break;
        case MousePosition::BottomRight:
            _currRect.setBottomRight(e->globalPos());
            setIGeometry(_currRect);
            break;
        case MousePosition::Normal:
            break;
        }
    }

    QWidget::mouseMoveEvent(e);

}
void ScalableFramelessWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->pos().y() > _headerHeight || e->button() != Qt::LeftButton)
    {
        return ;
    }
    if (_isFull)
    {
        showNormal();
        _isFull = false;
    }
    else
    {
        showFullScreen();
        _isFull = true;
    }
}
