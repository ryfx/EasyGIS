#include "locationwidget.h"
#include "ui_locationwidget.h"
#include <dlgimgpopup.h>

static int seed = time(0);
const QPoint LocationWidget::offset(12,24);

LocationWidget::LocationWidget(QWidget *parent, QPoint serverCoordinate) :
    QWidget(parent),
    ui(new Ui::LocationWidget)
{
    ui->setupUi(this);

    mapFrame = qobject_cast<MapTilesFrame *>(parent);

    assert(mapFrame != NULL);

    setServerCoordinate(serverCoordinate);
    connect(mapFrame, SIGNAL(viewportCoordinateChanged(QPoint,QPoint)), this, SLOT(checkVisibility()));
}

LocationWidget::~LocationWidget()
{
    delete ui;
}

void LocationWidget::moveToServerCoordinate(QPoint newPosition)
{
    setServerCoordinate(newPosition);
}

void LocationWidget::setServerCoordinate(QPoint newPosition)
{

    QPoint oldPos(m_serverPos);
    m_serverPos = newPosition;
    emit serverCoordinateChanged(newPosition, oldPos);

    if(!mapFrame->isInMap(newPosition, mapFrame->MAX_LEVEL))
        qDebug() << "Warning: requested Location Widget position is out of map area!";

    checkVisibility();
}

QPoint LocationWidget::getServerCoordinate() const
{
    return m_serverPos;
}


QPoint LocationWidget::getRelativeCoordinate(int scaleLevel) const
{
    return mapFrame->convertServerCoordinateToRelativeCoordinate(getServerCoordinate(), scaleLevel, mapFrame->MAX_LEVEL);
}

QPoint LocationWidget::getRandomPos() const
{
    static int i = 0;

    //图书馆，南门，11舍，二基楼
    QPoint positions[] = {QPoint(), QPoint(2450,950), QPoint(2550,150), QPoint(950,775), QPoint(2700,1100)};
    i++;
    if(i >= 5)
    {
        i = 1;
    }
    return positions[i];
}

QPoint LocationWidget::getTrulyRandomPos() const
{
    qsrand(seed);

    int x_range_max = mapFrame->scaleMaps[MapTilesFrame::MAX_LEVEL].width * 0.8;
    int y_range_max = mapFrame->scaleMaps[MapTilesFrame::MAX_LEVEL].height * 0.8;

    int x_range_min = mapFrame->scaleMaps[MapTilesFrame::MAX_LEVEL].width * 0.2;
    int y_range_min = mapFrame->scaleMaps[MapTilesFrame::MAX_LEVEL].height * 0.2;

    int x = (double)qrand() / (RAND_MAX + 1) * (x_range_max - x_range_min)
                  + x_range_min;
    int y = (double)qrand() / (RAND_MAX + 1) * (y_range_max - y_range_min)
            + y_range_min;

    seed = qrand();

    return QPoint(x, y);
}

QPoint LocationWidget::setRandomPos()
{
    QPoint pos(getRandomPos());
    setServerCoordinate(pos);
    return pos;
}

void LocationWidget::setPixmap(const QPixmap &pixmap)
{
    ui->imgLabel->setPixmap(pixmap);
}


void LocationWidget::move(const QPoint &point)
{
    QWidget::move(point);
}


void LocationWidget::checkVisibility()
{
    if(mapFrame->isInCurrentViewport(getServerCoordinate(), mapFrame->MAX_LEVEL))
    {
        setVisible(true);
        QPoint relativePos(mapFrame->convertServerCoordinateToRelativeCoordinate(getServerCoordinate(), mapFrame->getScaleLevel(), mapFrame->MAX_LEVEL));
        move(relativePos - offset);
        update();
    }
    else
    {
        setVisible(false);
    }
}


void LocationWidget::mousePressEvent(QMouseEvent *event)
{
    DlgImgPopup dlgImgPopup(this, getServerCoordinate());
    dlgImgPopup.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    dlgImgPopup.setAttribute(Qt::WA_TranslucentBackground);
    dlgImgPopup.move(event->globalPos() - QPoint(dlgImgPopup.width() / 3, dlgImgPopup.height() + 30));
    dlgImgPopup.exec();
}
