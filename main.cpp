#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <QDateTime>
#include <QScreen>
#include "scalableframelesswidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

//    QScreen *screen = QGuiApplication::primaryScreen();
//    QString filePathName = "full-";
//    filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
//    filePathName += ".jpg";
//    if(!screen->grabWindow(0).save(filePathName, "jpg"))

//    {
//        qDebug()<<"save full screen failed";
//    }

    ScalableFramelessWidget sw;
    w.setClipWindow(&sw);
    w.setWindowTitle(QObject::tr("AClip"));
    w.setFixedSize(QSize(210, 36));
    //w.setWindowFlag(Qt::Drawer, true);

    w.show();

    return a.exec();
}
