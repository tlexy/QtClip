#include "widget.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include "scalableframelesswidget.h"
#include <QThread>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      _sw(NULL)
{
    QHBoxLayout* hMainLayout = new QHBoxLayout;
    hMainLayout->setMargin(0);
    hMainLayout->setSpacing(2);

    _clipBtn = new QPushButton(tr("截图"));
    hMainLayout->addStretch();
    hMainLayout->addWidget(_clipBtn);
    hMainLayout->addStretch();

    setLayout(hMainLayout);

    connect(_clipBtn, &QPushButton::clicked, this, [=](){
        onClip();
    });

    _imgLbl = new QLabel();
    _pixLayout = new QVBoxLayout;
    _pixLayout->setMargin(0);
    _pixLayout->setSpacing(0);
    _pixLayout->addWidget(_imgLbl);
}

void Widget::onClip()
{
    if (_sw == NULL)
    {
        return ;
    }
    hide();
    QThread::msleep(180);
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pix = screen->grabWindow(0);
    _imgLbl->setPixmap(pix);
    _imgLbl->setVisible(true);
    //_imgLbl->showFullScreen();
    //_imgLbl->resize(pix.width()-16-2, pix.height()-31-8-1);
    //_imgLbl->show();
    _sw->setLayout(_pixLayout);
    _sw->resize(pix.width(), pix.height());
    _sw->show();
    qDebug() << "click";
}

void Widget::onCancel()
{
    qDebug() << "2333";
    _sw->hide();
    close();
}

void Widget::setClipWindow(QWidget* w)
{
    _sw = (ScalableFramelessWidget*)w;
    connect(_sw, &ScalableFramelessWidget::cancelClip, this, [=](){
       onCancel();
    });
}

Widget::~Widget()
{

}
