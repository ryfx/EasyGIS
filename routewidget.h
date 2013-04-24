#ifndef ROUTEWIDGET_H
#define ROUTEWIDGET_H

#include <QtGui>
#include <QtSql>
#include <QWidget>
#include <qmath.h>
#include "graphics.h"
#include "maptilesframe.h"
#include "locationwidget.h"
#include "soundmanager.h"

#include <assert.h>

class SoundManager;

class RouteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RouteWidget(QWidget *parent = 0);
    void initSqlDatabase();
    QList<QList<QPoint> > convertCoordinate(QList<QList<QPoint> > pointList);
    void initAbsPoints();

    QPoint findNearestSpotLocation(QPoint pos);
    QString findNearestSpotName(QPoint pos);

    QString findSpotName(QPoint pos, bool* isFound = 0);
    QPoint findSpotLocation(QString spotName, bool* isFound = 0);

    bool isPlacingStartLocation;   //用户是否正在放置起点
    bool isPlacingEndLocation;     //用户是否正在放置终点

    LocationWidget *userLocationWidget;
    LocationWidget *searchResultWidget;

    LocationWidget *startWidget;
    LocationWidget *endWidget;

    SoundManager *sm;

    bool isRoute;
    bool m_isRouteByCoor;//是否根据坐标点来进行寻路

    //本函数返回值所属坐标系是服务器绝对坐标系（最大放缩级别）
    QList<QPoint> getRouteTurningPoints(QPoint startPoint, QPoint endPoint);

signals:
    
public slots:
    //用于接收放缩倍数的槽函数
    void locationMultipleSlot(const QPoint& scalePos, int newScaleLevel, int oldScaleLevel);
    //获得实际窗口在服务器的位置，并藉此修正路径图层
    void clientWinLocSlot(QPoint newPos, QPoint oldPos);
    //接收关键点名字的槽函数
    void receiveNodeNameSlot(QString startName, QString endName);
    //接收关键点坐标的槽函数
    void receiveCoordinateSlot(QPoint startPosServerCoordinate, QPoint endPosServerCoordinate, bool isRouteByCoor = true);

    //判断起点和终点是否都被设置，如果都被设置则调用绘制路径函数
    void locationSet(bool isRouteByCoor = true);

private:
    QPen pen;
    Graphics* g;
    int multiple;
    int width;
    int height;

    QSqlDatabase db;

    QList<QList<QPoint> > absPointsList;//保存普通点绝对位置
    QPoint actualPos;//保存实际窗口位置
    MapTilesFrame* mapFrame;

    bool isStartLocationSet;  //起点是否已被设置好
    bool isEndLocationSet;    //终点是否已被设置好

    QPoint routeStartNode;
    QPoint routeEndNode;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);

};

#endif // ROUTEWIDGET_H
