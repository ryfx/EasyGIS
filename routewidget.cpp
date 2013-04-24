#include "routewidget.h"

RouteWidget::RouteWidget(QWidget *parent) :
    QWidget(parent)
{
    initSqlDatabase();
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setBrush(Qt::red);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    multiple = 1;
    this->actualPos = QPoint(0,500);
    this->absPointsList = QList<QList<QPoint> >();
    this->isRoute = FALSE;

    mapFrame = qobject_cast<MapTilesFrame *>(parent);
    assert(mapFrame != NULL);

    isPlacingStartLocation = false;
    isPlacingEndLocation = false;

    isStartLocationSet = false;
    isEndLocationSet = false;

    startWidget = NULL;
    endWidget = NULL;

    userLocationWidget = NULL;
    searchResultWidget = NULL;

//    this->isRoute = TRUE;
    this->m_isRouteByCoor = FALSE;
    connect(mapFrame,SIGNAL(scaleLevelChanged(QPoint,int,int)),this,SLOT(locationMultipleSlot(QPoint,int,int)));
    connect(mapFrame,SIGNAL(viewportCoordinateChanged(QPoint,QPoint)),this,SLOT(clientWinLocSlot(QPoint,QPoint)));

    sm = new SoundManager(mapFrame);

}

void RouteWidget::initSqlDatabase()
{
    if(!QFile::exists("mydb.db"))
    {
        QMessageBox::critical(0, tr("数据库连接失败"),
                              tr("无法连接上数据库，请检查数据库文件是否存在！"), QMessageBox::Ok);
        exit(0);
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydb.db");
    if (!db.open()) {
        QMessageBox::critical(0, tr("无法打开数据库"),
                              tr("无法打开本系统数据库，请检查数据库连接，并重新启动本系统！"), QMessageBox::Ok);
        exit(0);
    }

}

QList<QList<QPoint> > RouteWidget::convertCoordinate(QList<QList<QPoint> > pointListList)
{
    int height = mapFrame->scaleMaps[mapFrame->getScaleLevel()].height;
    int x;
    int y;
    QList<QList<QPoint> > pointListListTmp;
    for(int i=0; i<pointListList.size(); i++)
    {
        QList<QPoint> pointList = pointListList.value(i);
        QList<QPoint> pointListTmp;
        for(int j=0; j<pointList.size(); j++)
        {
            x = pointList.value(j).x()-this->actualPos.x();
            y = pointList.value(j).y()-(height-this->actualPos.y());
            pointListTmp.append(QPoint(x,y));
        }
        pointListListTmp.append(pointListTmp);
    }
    pointListList.clear();
    pointListList = pointListListTmp;
    return pointListList;

}

void RouteWidget::initAbsPoints()
{
    int x;
    int y;
    QString str,str1;
    QSqlQuery query;
    this->absPointsList.clear();
    for(int j=1; g->shortestRoute[j] != -1; j++)
    {
        QList<QPoint> pointListTmp;
        str.setNum(g->id[g->shortestRoute[j-1]]);
        str1.setNum(g->id[g->shortestRoute[j]]);
        //这里可能会出现通路方向问题
        if(!query.exec("SELECT CoordinateX,CoordinateY FROM Node WHERE startId="+str+" AND endId="+str1))
            qDebug() << "Error,Open failed!";
        query.next();
        if(!query.isValid())
        {
            if(!query.exec("SELECT CoordinateX,CoordinateY FROM Node WHERE startId="+str1+" AND endId="+str))
                qDebug() << "Error,Open failed!";
        }
        while(query.next())
        {
            x = query.value(0).toInt()*mapFrame->getScaleLevel()/6;
            y = (3000-query.value(1).toInt())*mapFrame->getScaleLevel()/6;
            qDebug() << x << ":" << y;
            pointListTmp.append(QPoint(x,y));
        }
        if(j == 1)
        {
            query.first();
            routeStartNode = QPoint(query.value(0).toInt(),query.value(1).toInt());
        }

        this->absPointsList.append(pointListTmp);
    }
    query.last();
    routeEndNode = QPoint(query.value(0).toInt(),query.value(1).toInt());

}

QPoint RouteWidget::findNearestSpotLocation(QPoint pos)//pos是服务器上s6的绝对坐标
{
    double minDistance = mapFrame->distance(QPoint(0,0),
                           QPoint(mapFrame->scaleMaps[MapTilesFrame::MAX_LEVEL].width,mapFrame->scaleMaps[MapTilesFrame::MAX_LEVEL].height));

    QPoint sp;

    int distance;

    QSqlQuery query;
    if(!query.exec("SELECT CoordinateX,CoordinateY FROM interestNode"))
        qDebug() << "Error,Open failed!";

    while(query.next())
    {
        QPoint spotPoint(query.value(0).toInt(), query.value(1).toInt());
        distance = mapFrame->distance(spotPoint, pos);

        if(distance < minDistance)
        {
            minDistance = distance;
            sp = spotPoint;
        }
    }

    //qDebug() << "Nearest Spot of " << pos << " is :" << sp;
    return sp;
}

QString RouteWidget::findNearestSpotName(QPoint pos)
{
    QPoint nearestSpotLocation(findNearestSpotLocation(pos));
    bool ok = false;

    QString nearestSpotName(findSpotName(nearestSpotLocation, &ok));

    assert(ok == true);

    return nearestSpotName;
}

QString RouteWidget::findSpotName(QPoint pos, bool *isFound)
{
    QString strX,strY;
    strX.setNum(pos.x());
    strY.setNum(pos.y());

    QSqlQuery query;
    if(!query.exec("SELECT name FROM interestNode where CoordinateX="+strX+" and CoordinateY="+strY))
        qDebug() << "Error,Open failed!";
    query.next();

    bool internal_isFound = query.isValid();

    if(isFound != NULL)
    {
        *isFound = internal_isFound;
    }

    if(internal_isFound)
    {
        return QString(query.value(0).toString());
    }
    else
    {
        return QString();
    }

}

QPoint RouteWidget::findSpotLocation(QString spotName, bool* isFound)
{

    QSqlQuery query;
    if(!query.exec("SELECT CoordinateX,CoordinateY FROM interestNode where name='"+spotName+"'"))
        qDebug() << "Error,Open failed!";
    query.next();

    bool internal_isFound = query.isValid();

    if(isFound != NULL)
    {
        *isFound = internal_isFound;
    }

    if(internal_isFound)
    {
        return QPoint(query.value(0).toInt(),query.value(1).toInt());
    }
    else
    {
        return QPoint();
    }
}

QList<QPoint> RouteWidget::getRouteTurningPoints(QPoint startPoint, QPoint endPoint)
{
    Graphics* g = new Graphics(startPoint,endPoint);
    g->findShortestPath();
    QList<QPoint> pointListTmp;

    QString str, str1;
    QSqlQuery query;
    int x, y;
    for(int j=1; g->shortestRoute[j] != -1; j++)
    {
        str.setNum(g->id[g->shortestRoute[j-1]]);
        str1.setNum(g->id[g->shortestRoute[j]]);
        //这里可能会出现通路方向问题
        if(!query.exec("SELECT CoordinateX,CoordinateY FROM Node WHERE startId="+str+" AND endId="+str1))
            qDebug() << "Error,Open failed!";
        query.next();
        if(!query.isValid())
        {
            if(!query.exec("SELECT CoordinateX,CoordinateY FROM Node WHERE startId="+str1+" AND endId="+str))
                qDebug() << "Error,Open failed!";
        }
        while(query.next())
        {
            x = query.value(0).toInt();
            y = query.value(1).toInt();
            pointListTmp.append(QPoint(x,y));
        }
    }

    return pointListTmp;
}

void RouteWidget::locationMultipleSlot(const QPoint &scalePos, int newScaleLevel, int oldScaleLevel)
{
    Q_UNUSED(scalePos)

    int x;
    int y;
    QList<QList<QPoint> > pointListListTmp;
    for(int i=0; i<this->absPointsList.size(); i++)
    {
        QList<QPoint> pointListTmp;
        QList<QPoint> pointList = this->absPointsList.value(i);
        for(int j=0; j<pointList.size(); j++)
        {
            x = (pointList.value(j).x()*newScaleLevel)/oldScaleLevel;
            y = (pointList.value(j).y()*newScaleLevel)/oldScaleLevel;
            pointListTmp.append(QPoint(x,y));
        }
        pointListListTmp.append(pointListTmp);
    }
    this->absPointsList = pointListListTmp;
    this->update();
}


void RouteWidget::clientWinLocSlot(QPoint newPos, QPoint oldPos)
{
    Q_UNUSED(oldPos)

    this->actualPos = newPos;
    this->update();
}

void RouteWidget::receiveNodeNameSlot(QString startName, QString endName)
{
    this->g = new Graphics(startName, endName);
    this->g->findShortestPath();
    //添加代码
    initAbsPoints();
    this->isRoute = TRUE;

    this->m_isRouteByCoor = FALSE;

    this->update();
}

void RouteWidget::receiveCoordinateSlot(QPoint startPosServerCoordinate, QPoint endPosServerCoordinate, bool isRouteByCoor)
{
    QPoint sp = findNearestSpotLocation(startPosServerCoordinate);
    QPoint ep = findNearestSpotLocation(endPosServerCoordinate);

    g = new Graphics(sp, ep);
    g->findShortestPath();
    initAbsPoints();
    this->isRoute = TRUE;
    this->m_isRouteByCoor = isRouteByCoor;//将是否利用坐标点寻路设置为TRUE
    this->update();

}

void RouteWidget::locationSet(bool isRouteByCoor)
{
//    startWidget = mapFrame->findChild<LocationWidget* >("startLocation");
//    endWidget = mapFrame->findChild<LocationWidget* >("endLocation");

    if(startWidget != NULL && endWidget != NULL)
    {
        receiveCoordinateSlot(startWidget->getServerCoordinate(),endWidget->getServerCoordinate(), isRouteByCoor);
    }
}

void RouteWidget::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    QPoint sp;
    QPoint ep;
    QList<QList<QPoint> > pointListList = convertCoordinate(this->absPointsList);
    if( this->isRoute )
    {
        paint.setPen(pen);
        for(int i=0; i<pointListList.size(); i++)
        {
            QList<QPoint> pointList = pointListList.value(i);
            sp = pointList.value(0);
            for(int j=1; j<pointList.size(); j++)
            {
                qDebug() << sp.x() << "^^^" << sp.y();
                ep = pointList.value(j);
                paint.drawLine(sp,ep);
                sp = ep;
            }
        }
        /*****************************/
        if(m_isRouteByCoor)//画出起点和终点
        {
            paint.setPen(pen);

            QPoint routeStartNodeRelative(mapFrame->convertServerCoordinateToRelativeCoordinate(routeStartNode, mapFrame->getScaleLevel(), mapFrame->MAX_LEVEL));
            QPoint routeEndNodeRelative(mapFrame->convertServerCoordinateToRelativeCoordinate(routeEndNode, mapFrame->getScaleLevel(), mapFrame->MAX_LEVEL));

            QPoint startPointRelative(startWidget->getRelativeCoordinate(mapFrame->getScaleLevel()));
            QPoint endPointRelative(endWidget->getRelativeCoordinate(mapFrame->getScaleLevel()));

            paint.drawLine(routeStartNodeRelative, startPointRelative);
            paint.drawLine(routeEndNodeRelative, endPointRelative);


//            int height = mapFrame->scaleMaps[mapFrame->getScaleLevel()].height;
//            QPoint pointTmp(this->actualPos.x(),height-this->actualPos.y());
//            QPoint spD = spDesign-pointTmp;
//            QPoint spA = spActual-pointTmp;
//            QPoint epD = epDesign-pointTmp;
//            QPoint epA = epActual-pointTmp;
//            paint.drawLine(spD,spA);
//            paint.drawLine(epD,epA);
//            qDebug() << "spDesign: " << spDesign << ", spActual: " << spActual;
//            qDebug() << "epDesign: " << epActual << ", epActual: " << spActual;
        }
        /***************************/
    }
}

void RouteWidget::mousePressEvent(QMouseEvent *event)
{
    if(isPlacingStartLocation)
    {
        if(startWidget != NULL)
        {
            delete startWidget;
            startWidget = NULL;
        }
        startWidget = new LocationWidget(mapFrame);
        startWidget->setPixmap(QPixmap(":/images/start.png"));
        startWidget->setServerCoordinate(mapFrame->convertRelativeCoordinateToServerCoordinate(event->pos(), mapFrame->MAX_LEVEL, mapFrame->getScaleLevel()));

        qDebug() << "startWidget coord:" << startWidget->getServerCoordinate();

        unsetCursor();
        isPlacingStartLocation = false;
        locationSet();
    }
    else if(isPlacingEndLocation)
    {
        if(endWidget != NULL)
        {
            delete endWidget;
            endWidget = NULL;
        }
        endWidget = new LocationWidget(mapFrame);
        endWidget->setPixmap(QPixmap(":/images/end.png"));
        endWidget->setServerCoordinate(mapFrame->convertRelativeCoordinateToServerCoordinate(event->pos(), mapFrame->MAX_LEVEL, mapFrame->getScaleLevel()));

        unsetCursor();
        isPlacingEndLocation = false;
        locationSet();
    }
    else
    {
        event->ignore();
    }
}
