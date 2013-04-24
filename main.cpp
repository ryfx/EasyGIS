#include "widget.h"
#include <QApplication>
#include <QTextCodec>
#include <QString>
#include <QStringList>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    QApplication a(argc, argv);

    qApp->addLibraryPath(".");

    Widget w;

    //w.setGraphicsEffect(new QGraphicsDropShadowEffect);
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.setAttribute(Qt::WA_TranslucentBackground);

    QPropertyAnimation *animation = new QPropertyAnimation(&w, "geometry"); //打开软件卷轴展开效果
    animation->setDuration(1000);
    animation->setStartValue(QRect(605, 0, 0, 770));
    animation->setEndValue(QRect(0, 0, 1210, 770));
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start();

    w.show();
    return a.exec();
}



