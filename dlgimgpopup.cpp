#include "dlgimgpopup.h"
#include "ui_dlgimgpopup.h"

DlgImgPopup::DlgImgPopup(QWidget *parent, QPoint locationCoord) :
    QDialog(parent),
    locationPoint(locationCoord),
    ui(new Ui::DlgImgPopup)
{
    ui->setupUi(this);

    mapFrame = qobject_cast<MapTilesFrame *>(this->parentWidget()->parentWidget());
    assert(mapFrame != NULL);

    //connect(&mapFrame->provider(), SIGNAL(readyImage(QString)), this, SLOT(update()));

    imgIndex = 0;

    //ui->imgPopupLabel->setStyleSheet("border-image: url(:/images/qPictur.jpg);");

    QSqlQuery query;

    QString queryString("SELECT name, ImageName FROM interestNode WHERE CoordinateX=" +
                        QString().setNum(locationPoint.x()) + " AND CoordinateY=" + QString().setNum(locationPoint.y()));

    query.exec(queryString);
    query.next();

    assert(query.isValid());

    QString spotName = query.value(0).toString();
    QString imageNames = query.value(1).toString();

    ui->spotNameLabel->setText(spotName);

    if(imageNames == "none" || imageNames.isEmpty())
    {
        ui->imgPopupLabel->setPixmap(QPixmap(":/images/notile.bmp"));
    }
    else
    {
        imgNameList = imageNames.split("#");

//        QString imgName;
//        foreach(imgName, imgNameList)
//        {
//            mapFrame->provider().getTile(QString("/SpotImages/" + imgName));
//        }

        autoChangeImg();

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(autoChangeImg()));
        timer->start(2500);
    }

//    QString imgName;
//    foreach(imgName, imgNameList)
//    {
//        if(imgName == "none")
//        {
//            break;
//        }
//        imgList.append();
//    }

}

DlgImgPopup::~DlgImgPopup()
{
    delete ui;
}

void DlgImgPopup::mousePressEvent(QMouseEvent *)
{
    close();
}

void DlgImgPopup::autoChangeImg()
{
    QString imgName = imgNameList[imgIndex];
    QImage* img = mapFrame->provider().getTile(QString("/SpotImages/" + imgName), true);
//    while(!img)
//    {
//        img = mapFrame->provider().getTile(QString("/SpotImages/" + imgName), true);
//        QCoreApplication::processEvents();
//    }
    if(img)
    {
        QPixmap pixmap(QPixmap::fromImage(*img));
        ui->imgPopupLabel->setPixmap(pixmap);
        delete img;
        img = NULL;
        imgIndex++;
        if(imgIndex >= imgNameList.length())
        {
            imgIndex = 0;
        }
    }

}

