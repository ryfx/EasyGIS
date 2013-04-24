#ifndef MAPTILESFRAME_H
#define MAPTILESFRAME_H

//#include <QFrame>
#include <QtGui>
#include "zoomeffect.h"
#include "maptilesprovider.h"

#include "locationwidget.h"

typedef class TagScaleMap {
public:
    int width;
    int height;
} ScaleMap;

namespace Ui {
class ScaleMap;
class MapTilesFrame;
}

class MapTilesFrame : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool isMoving READ isMoving WRITE setMoving NOTIFY movingStatusChanged)
    Q_PROPERTY(QPoint viewportCoordinate READ getViewportCoordinate WRITE setViewportCoordinate NOTIFY viewportCoordinateChanged)

    //Q_PROPERTY(int scaleLevel READ getScaleLevel WRITE setScaleLevel NOTIFY scaleLevelChanged)

public:
    explicit MapTilesFrame(QWidget *parent = 0);
    ~MapTilesFrame();

    //QString mapFilesPath;

    static const int MAX_LEVEL = 6;

    //这里要舍弃scaleMaps[0]！
    ScaleMap scaleMaps[MAX_LEVEL + 1];

    int tileWidth;
    int tileHeight;

    int timeToGo;

    bool isMoving() const;
    void setMoving(bool movingStatus = true);

    QPoint getViewportCoordinate() const;
    void setViewportCoordinate(const QPoint& requestedPos);

    int getViewportWidth() const;
    int getViewportHeight() const;
    QSize getViewportSize() const;
    //QRect getViewportRect() const;

    int getScaleLevel() const;
    void setScaleLevel(const QPoint& scalePos, int scaleLevel);
    void zoomIn(const QPoint& zoomPos);
    void zoomOut(const QPoint& zoomPos);
    void zoomIn();
    void zoomOut();

    QPoint convertServerCoordinateToDifferentScaleLevel(QPoint serverCoordinate, int newScaleLevel, int oldScaleLevel);

    QPoint convertRelativeCoordinateToServerCoordinate(QPoint relativeCoordinate, int newScaleLevel, int oldScaleLevel);
    QPoint convertRelativeCoordinateToServerCoordinate(QPoint relativeCoordinate);

    QPoint convertServerCoordinateToRelativeCoordinate(QPoint serverCoordinate);
    QPoint convertServerCoordinateToRelativeCoordinate(QPoint serverCoordinate, int newScaleLevel, int oldScaleLevel);

    bool isInCurrentViewport(QPoint serverCoordinate, int scaleLevel);
    bool isInMap(QPoint serverCoordinate, int scaleLevel);

    double distance(QPoint point1, QPoint point2);

    MapTilesProvider& provider();

private:
    Ui::MapTilesFrame *ui;

    MapTilesProvider tileProvider;

    int m_scaleLevel;

    bool m_isMoving;
    QPoint m_viewportCoordinate; //客户端地图可视区域的左上角坐标

    bool isViewportBiggerThanWholeMap;


    QPoint dragPosition;

    //为了处理客户端地图可视区域边缘存在半块瓦片的情况，需要读取周围完整的瓦片，构成的更大区域存放在此变量中
    QImage *biggerMapRect;
    //从上面大图像中，截取的客户端实际显示区域
    QRect clippedMapRect;


    bool prepareViewport(QPoint newPos);

    ZoomEffect* createZoomEffect(const QPoint& pos);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:
    void scaleLevelChanged(const QPoint& scalePos, int newScaleLevel, int oldScaleLevel);
    void movingStatusChanged(bool newMovingStatus);
    void viewportCoordinateChanged(QPoint newPos, QPoint oldPos);

private slots:
    void updateMouseCursor(bool currentMovingStatus);

    //bool prepareViewport(QPoint newPos);

    void adjustViewportOnZoom(const QPoint& scalePos, int newScaleLevel, int oldScaleLevel);

};

#endif // MAPTILESFRAME_H
