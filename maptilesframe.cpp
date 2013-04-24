/*============================================================
 * 地图瓦片显示模块
 * 功能 ： 计算需要显示的地图区域，显示这些地图瓦片，
 *        并提供相应 API 接口，响应用户界面上放缩地图、移动地图视口等操作
 * 模块重要性： 重要
 *
 * 作者 ： 刘梓懿
 *
 * -----------------------------------------------------------
 * 变更履历 ：
 *
 ============================================================*/

#include "maptilesframe.h"
#include "ui_maptilesframe.h"

MapTilesFrame::MapTilesFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MapTilesFrame)
{
    ui->setupUi(this);

    connect(this, SIGNAL(movingStatusChanged(bool)),
            this, SLOT(updateMouseCursor(bool)));
    connect(this, SIGNAL(viewportCoordinateChanged(QPoint,QPoint)),
            this, SLOT(update()));
    connect(this, SIGNAL(scaleLevelChanged(const QPoint&,int,int)),
            this, SLOT(adjustViewportOnZoom(const QPoint&,int,int)));

    connect(&tileProvider, SIGNAL(mapTypeChanged(QString)), this, SLOT(update()));
    connect(&tileProvider, SIGNAL(dataSourceChanged(DataSource)), this, SLOT(update()));
    connect(&tileProvider, SIGNAL(serverAddressChanged(QString)), this, SLOT(update()));
    connect(&tileProvider, SIGNAL(tileFileTypeChanged(QString)), this, SLOT(update()));
    connect(&tileProvider, SIGNAL(readyTile(QString)), this, SLOT(update()));


//    connect(this, SIGNAL(scaleLevelChanged(QPoint,int,int)),
//            this,SLOT(setAppropriateSize()));

    //mapFilesPath = QCoreApplication::applicationDirPath() + "/map";

    tileWidth = 375;
    tileHeight = 250;

    for(int i = 1; i <= MAX_LEVEL; i++)
    {
        scaleMaps[i].width = 2 * i * tileWidth;
        scaleMaps[i].height = 2 * i * tileHeight;
    }

    biggerMapRect = new QImage();
    //biggerMapRect = QImage(9999, 9999, QImage::Format_RGB16);

    m_viewportCoordinate = QPoint(0,height());
    //setViewportCoordinate(QPoint(0,height()));
    setMoving(false);

    //先设置scaleLevel为一个不存在的值 0 ，然后调用zoomIn使视口放缩级别调整到到最小可视级别
    m_scaleLevel = 1;
    //zoomIn();

    setViewportCoordinate(QPoint(0,height()));

}



MapTilesFrame::~MapTilesFrame()
{
    delete ui;
}

void MapTilesFrame::setMoving(bool movingStatus)
{
    m_isMoving = movingStatus;
    emit movingStatusChanged(movingStatus);
}

QPoint MapTilesFrame::getViewportCoordinate() const
{
    return m_viewportCoordinate;
}

void MapTilesFrame::setViewportCoordinate(const QPoint &requestedPos)
{
    QPoint adjustedPoint;
    QPoint oldPoint(m_viewportCoordinate);

    adjustedPoint.rx() = qBound(0, requestedPos.x(), scaleMaps[getScaleLevel()].width - getViewportWidth());
    adjustedPoint.ry() = qBound(getViewportHeight(), requestedPos.y(), scaleMaps[getScaleLevel()].height);

    m_viewportCoordinate = adjustedPoint;

    qDebug() << "Requested viewportPoint: " << requestedPos << ", adjustedPoint" << adjustedPoint;
    emit viewportCoordinateChanged(adjustedPoint, oldPoint);

}

int MapTilesFrame::getViewportWidth() const
{
    if(width() > scaleMaps[getScaleLevel()].width)
    {
        //isViewportBiggerThanWholeMap = true;
        return scaleMaps[getScaleLevel()].width;
    }
    else
    {
        //isViewportBiggerThanWholeMap = false;
        return width();
    }
}

int MapTilesFrame::getViewportHeight() const
{
    if(height() > scaleMaps[getScaleLevel()].height)
    {
        //isViewportBiggerThanWholeMap = true;
        return scaleMaps[getScaleLevel()].height;
    }
    else
    {
        //isViewportBiggerThanWholeMap = false;
        return height();
    }
}

QSize MapTilesFrame::getViewportSize() const
{
    return QSize(getViewportWidth(), getViewportHeight());
}

int MapTilesFrame::getScaleLevel() const
{
    return m_scaleLevel;
}

void MapTilesFrame::setScaleLevel(const QPoint& scalePos, int scaleLevel)
{
    int oldLevel = m_scaleLevel;

    if(scaleLevel >= 1 && scaleLevel <= MAX_LEVEL)
    {
        m_scaleLevel = scaleLevel;
        emit scaleLevelChanged(scalePos, scaleLevel, oldLevel);
    }
}

void MapTilesFrame::zoomIn(const QPoint& zoomPos)
{
    setScaleLevel(zoomPos, getScaleLevel() + 1);
}

void MapTilesFrame::zoomOut(const QPoint& zoomPos)
{
    setScaleLevel(zoomPos, getScaleLevel() - 1);
}

void MapTilesFrame::zoomIn()
{
    qDebug() << "zooming IN . . . viewport coordinate: " << getViewportCoordinate();
    qDebug() << "viewport size: " << size();

//    ZoomEffect *ze = createZoomEffect(QPoint(width() / 2, height() / 2));
//    ze->zoomInEffect();

    //以视口中心点为视点缩放（服务器坐标系）
    QPoint viewportCenterPoint(getViewportCoordinate().x() + getViewportWidth() / 2,
                               getViewportCoordinate().y() - getViewportHeight() / 2);
    zoomIn(viewportCenterPoint);
}

void MapTilesFrame::zoomOut()
{
    qDebug() << "zooming OUT . . . viewport coordinate: " << getViewportCoordinate();
    qDebug() << "viewport size: " << size();

//    ZoomEffect *ze = createZoomEffect(QPoint(width() / 2, height() / 2));
//    ze->zoomOutEffect();

    //以视口中心点为视点缩放（服务器坐标系）
    QPoint viewportCenterPoint(getViewportCoordinate().x() + getViewportWidth() / 2,
                               getViewportCoordinate().y() - getViewportHeight() / 2);
    zoomOut(viewportCenterPoint);
}

QPoint MapTilesFrame::convertServerCoordinateToDifferentScaleLevel(QPoint serverCoordinate, int newScaleLevel, int oldScaleLevel)
{
    return serverCoordinate * newScaleLevel / oldScaleLevel;
}

QPoint MapTilesFrame::convertRelativeCoordinateToServerCoordinate(QPoint relativeCoordinate, int newScaleLevel, int oldScaleLevel)
{
    QPoint nowPos(relativeCoordinate.x() * getViewportWidth() / width() + getViewportCoordinate().x(),
                   getViewportCoordinate().y() - relativeCoordinate.y() * getViewportHeight() / height());
    return nowPos * newScaleLevel / oldScaleLevel;
}


QPoint MapTilesFrame::convertRelativeCoordinateToServerCoordinate(QPoint relativeCoordinate)
{
    return convertRelativeCoordinateToServerCoordinate(relativeCoordinate, 1, 1);
}

QPoint MapTilesFrame::convertServerCoordinateToRelativeCoordinate(QPoint serverCoordinate)
{
    return convertServerCoordinateToRelativeCoordinate(serverCoordinate, 1, 1);
}

QPoint MapTilesFrame::convertServerCoordinateToRelativeCoordinate(QPoint serverCoordinate, int newScaleLevel, int oldScaleLevel)
{
    QPoint nowPos(serverCoordinate * newScaleLevel / oldScaleLevel);
    return QPoint(nowPos.x() - getViewportCoordinate().x(), getViewportCoordinate().y() - nowPos.y());
}

bool MapTilesFrame::isInCurrentViewport(QPoint serverCoordinate, int scaleLevel)
{
    QPoint relativePoint(convertServerCoordinateToRelativeCoordinate(serverCoordinate, getScaleLevel(), scaleLevel));
    return QRect(0,0,getViewportWidth(),getViewportHeight()).contains(relativePoint);
}

bool MapTilesFrame::isInMap(QPoint serverCoordinate, int scaleLevel)
{
    QRect mapRect(0, 0, scaleMaps[scaleLevel].width, scaleMaps[scaleLevel].height);
    return mapRect.contains(serverCoordinate);
}

double MapTilesFrame::distance(QPoint point1, QPoint point2)
{
    QPoint diff(point1 - point2);
    return sqrt(pow((double)diff.x(), 2) + pow((double)diff.y(), 2));
}

MapTilesProvider &MapTilesFrame::provider()
{
    return tileProvider;
}

bool MapTilesFrame::isMoving() const
{
    return m_isMoving;
}

void MapTilesFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        timeToGo = 4;
        setMoving(true);

        dragPosition = QPoint(event->x(), this->height() - event->y());
        event->accept();
    }
}

void MapTilesFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (isMoving())
    {
        //当下面的计数器达到一定次数时，才响应一次鼠标移动事件
        ++timeToGo;
        if(timeToGo >= 5)
        {
            timeToGo = 0;

            setViewportCoordinate(getViewportCoordinate() - QPoint(event->x(), this->height() - event->y()) + dragPosition);
            //move(event->globalPos() - dragPosition);
            event->accept();
        }
    }
}

void MapTilesFrame::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setMoving(false);
    event->accept();
}

void MapTilesFrame::wheelEvent(QWheelEvent *event)
{
    ZoomEffect *ze = createZoomEffect(event->pos());
    QPoint zoomPos(convertRelativeCoordinateToServerCoordinate(event->pos()));

    if (event->orientation() == Qt::Vertical) {
        if(event->delta() > 0)
        {
            ze->zoomInEffect();
            zoomIn(zoomPos);
        }
        else
        {
            ze->zoomOutEffect();
            zoomOut(zoomPos);
        }
    }

    event->accept();
}

void MapTilesFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

//    if(getScaleLevel() == 1)
//    {
//        setFixedSize(750,500);
//        updateGeometry();
//    }
//    else
//    {
//        setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
//        updateGeometry();
//    }

    prepareViewport(getViewportCoordinate());

    if(isViewportBiggerThanWholeMap)
    {
        int scaleLevel = getScaleLevel();
        QPoint startDrawPoint((width() - scaleMaps[scaleLevel].width) / 2,
                (height() - scaleMaps[scaleLevel].height) / 2);
        p.drawImage(startDrawPoint, *biggerMapRect, clippedMapRect);
    }
    else
    {
        p.drawImage(rect(), *biggerMapRect, clippedMapRect);
    }

}

void MapTilesFrame::resizeEvent(QResizeEvent *event)
{
    qDebug() << "resizeEvent called, newSize: " << event->size() << ", oldSize: " << event->oldSize();

    qDebug() << "viewport Coordinate: " << getViewportCoordinate();

    //update();
    //setViewportCoordinate(getViewportCoordinate());

    //通过重新设置视口坐标的方法，使视口中内容得到更新
    //setViewportCoordinate(getViewportCoordinate());

}

/*============================================================
 * 更新鼠标指针样式函数
 *（Qt 槽函数）
 * 功能说明  ： 根据当前用户是否在拖动地图，变换相对应的鼠标指针样式
 * 函数重要性 ： 一般
 *
 * 参数说明 ： 布尔变量，用户在移动视口时该变量为真，反之为假
 * 返回值  :  无
 * 作者    ： 杨飞、刘梓懿
 * -----------------------------------------------------------
 * 变更履历 ：
 *
 ============================================================*/
void MapTilesFrame::updateMouseCursor(bool currentMovingStatus)
{
    //qDebug() << "updateMousecursor called, CurrentMovingStatus = " << currentMovingStatus;
    if(currentMovingStatus)
    {
        setCursor(Qt::ClosedHandCursor);
    }
    else
    {
        setCursor(Qt::OpenHandCursor);
    }
}

/*============================================================
 * 视口准备函数
 *（Qt 槽函数）
 * 功能说明 ： 计算并加载客户端地图视口区域要显示的地图瓦片
 *           供 paintEvent 使用
 * 函数重要性： 重要
 *
 * 参数说明 ： 新的视口坐标
 *           （视口坐标为视口的左上角点坐标，以服务器坐标系为基准）
 * 返回值  :  无
 * 作者    ： 刘梓懿
 * -----------------------------------------------------------
 * 变更履历 ：
 *
 ============================================================*/
bool MapTilesFrame::prepareViewport(QPoint newPos)
{
    //本函数需要完成的具体任务是，计算并准备好biggerMapRect和clippedMapRect，供 paintEvent 使用

    qDebug() << "newPos: " << newPos;

    static QString tilePath("/s%1/s%2_%3-%4_%5-%6");
    //static QImage tile(tileWidth, tileHeight, QImage::Format_RGB16);
    static QImage noDataImage(":/images/notile.bmp");    //显示“暂无数据”的图片占位符，正好一个瓦片大小的

    int scaleLevel = getScaleLevel();     //当前缩放级别

    int w = width();
    int h = height();

    int mapWidth = scaleMaps[scaleLevel].width;
    int mapHeight = scaleMaps[scaleLevel].height;

     //判断用户显示视口大小是否大于当前缩放级别的整张地图大小
    if(w > mapWidth || h > mapHeight)
    {
        qDebug() << "viewport is bigger that whole map!";
        qDebug() << "viewportSize: " << size() << ", mapWidth: " << mapWidth << ", mapHeight: " << mapHeight;
        isViewportBiggerThanWholeMap = true;

        w = mapWidth;
        h = mapHeight;
    }
    else
    {
        isViewportBiggerThanWholeMap = false;
    }

    //新视口坐标的X和Y是否在地图网格线上（即X/Y坐标值是否能被被瓦片宽度/高度整除）
    //此判断在后面代码中会用到
    bool isSpecialX = false;
    bool isSpecialY = false;
    if(newPos.x() % tileWidth == 0)
    {
        isSpecialX = true;
        //qDebug() << "Special X!!!";
    }
    if(newPos.y() % tileHeight == 0)
    {
        isSpecialY = true;
        //qDebug() << "Special Y!!!";
    }

    //此为视口坐标点所在瓦片的左下角点，以下几行代码可以成功实现获取该点坐标
    QPoint tileBottomLeftCorner;
    tileBottomLeftCorner.rx() = newPos.x() - newPos.x() % tileWidth;
    tileBottomLeftCorner.ry() = newPos.y() - newPos.y() % tileHeight;
    if(isSpecialY)
    {
        tileBottomLeftCorner.ry() -= tileHeight;
    }

    //计算biggerMapRect占了几行、几列瓦片
    //numRows表示所占行数，numColumns表示所占列数
    //TODO: 算法有待优化
    int numRows;
    int numColumns;
    if(isViewportBiggerThanWholeMap)
    {
        numRows = mapHeight / tileHeight;
        numColumns = mapWidth / tileWidth;
    }
    else
    {
        int ceiling = newPos.y() / tileHeight * tileHeight + (newPos.y() % tileHeight == 0 ? 0 : tileHeight);
        int bottom = (newPos.y() - h) / tileHeight * tileHeight;
        numRows = (ceiling - bottom) / tileHeight;

        int rightX = newPos.x() + width();
        int rightOffset = ((rightX % tileWidth == 0) ? 0 : tileWidth);
        int leftBorder = newPos.x() - newPos.x() % tileWidth;
        int rightBorder = rightX / tileWidth * tileWidth + rightOffset;
        numColumns = (rightBorder - leftBorder) / tileWidth;
    }


    //biggerMapRect需要的大小
    //如果biggerMapRect不够大，则重新创建足够大的biggerMapRect
    QSize biggerMapRectSize(numColumns * tileWidth, numRows * tileHeight);
    if(biggerMapRect->width() < biggerMapRectSize.width()
            || biggerMapRect->height() < biggerMapRectSize.height())
    {
        delete biggerMapRect;
        biggerMapRect = new QImage(biggerMapRectSize, QImage::Format_RGB16);
    }


    QPainter painter(biggerMapRect);  //painter不能设为static！
    QString finalPath;     //最终的瓦片文件地址

    //biggerMapRect要加载的每个瓦片的左下角坐标和右上角坐标
    QPoint tempBL;
    QPoint tempTR;

//    qDebug() << "tileBottomLeftCorner:" << tileBottomLeftCorner;
//    qDebug() << "viewportCoordinate:" << newPos;
//    qDebug() << "viewportSize:" << size();
//    qDebug() << "numRows: " << numRows << ", numColumns: " << numColumns;

    QImage *tile = NULL;
    for(int m = 0; m < numRows; m++)
    {
        for(int n = 0; n < numColumns; n++)
        {
            tempBL.rx() = tileBottomLeftCorner.x() + n * tileWidth;
            tempBL.ry() = tileBottomLeftCorner.y() - m * tileHeight;
            tempTR.rx() = tempBL.x() + tileWidth;
            tempTR.ry() = tempBL.y() + tileHeight;

            //qDebug() << "tempBL" << tempBL;
            //qDebug() << "tempTR" << tempTR;

            finalPath = tilePath.arg(scaleLevel)
                    .arg(scaleLevel)
                    .arg(tempBL.x())
                    .arg(tempBL.y())
                    .arg(tempTR.x())
                    .arg(tempTR.y());
            //qDebug() << "loading tile(" << m << "," << n << "): " << finalPath;

            tile = tileProvider.getTile(finalPath);
            if(!tile)
            {
                qDebug() << "No tile data: " << finalPath;
                tile = &noDataImage;
                painter.drawImage(n * tileWidth, m * tileHeight, *tile);
            }
            else
            {
                painter.drawImage(n * tileWidth, m * tileHeight, *tile);
                delete tile;
                tile = NULL;
            }

        }
    }

    //此为视口在biggerMapRect中的相对坐标（坐标系为Qt坐标系，非服务器坐标系）
    QPoint viewportRelativeCoodinate(newPos.x() % tileWidth,
                                     isSpecialY ? 0 : (tileHeight - newPos.y() % tileHeight));

    //此为从biggerMapRect裁剪出来的，最终要呈现在屏幕上的显示矩形
    clippedMapRect = QRect(viewportRelativeCoodinate,
                           isViewportBiggerThanWholeMap ? QSize(mapWidth, mapHeight): size());

    qDebug() << "biggerMapRect" << biggerMapRect->size();
    qDebug() << "clippedMapRect: " << clippedMapRect;

    return isViewportBiggerThanWholeMap;
}

ZoomEffect *MapTilesFrame::createZoomEffect(const QPoint &pos)
{
    ZoomEffect *ze = new ZoomEffect(this);
    ze->move(pos.x() - ze->width() / 2, pos.y() - ze->height() / 2);
    QTimer::singleShot(400,ze,SLOT(deleteLater()));
    return ze;
}

void MapTilesFrame::adjustViewportOnZoom(const QPoint &scalePos, int newScaleLevel, int oldScaleLevel)
{
    qDebug() << "adjustViewportOnZoom called. scalePos: " << scalePos;
    qDebug() << "newScaleLevel: " << newScaleLevel << ", oldScaleLevel: " << oldScaleLevel;

    //用户指定的缩放点（scalePos）相对于视口位置（viewportCoordinate）的相对偏移
    QPoint diff(scalePos - getViewportCoordinate());

    //用户指定的缩放点（scalePos）在新的放缩级别下的坐标位置（以服务器坐标系为基准）
    QPoint newScalePos(scalePos * newScaleLevel / oldScaleLevel);

    setViewportCoordinate(newScalePos - diff);
}
