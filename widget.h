#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;
class ScalableFramelessWidget;
class QVBoxLayout;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    void setClipWindow(QWidget* w);
    ~Widget();

private:
    QPushButton* _clipBtn;
    QLabel* _imgLbl;
    ScalableFramelessWidget* _sw;
    QVBoxLayout* _pixLayout;

public slots:
    void onClip();
    void onCancel();
};

#endif // WIDGET_H
