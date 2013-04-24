#ifndef DLGIMGPOPUP_H
#define DLGIMGPOPUP_H

#include <QDialog>
#include <QtGui>
#include <QtSql>
#include <assert.h>
#include "maptilesframe.h"

namespace Ui {
class DlgImgPopup;
}

class DlgImgPopup : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgImgPopup(QWidget *parent, QPoint locationCoord);
    ~DlgImgPopup();

protected:
    void mousePressEvent(QMouseEvent *);
    
private:
    Ui::DlgImgPopup *ui;

    MapTilesFrame *mapFrame;

    int imgIndex;

    QStringList imgNameList;

    QPoint locationPoint;

    QList<QPixmap> imgList;

    QTimer *timer;
public slots:
    void autoChangeImg();
};

#endif // DLGIMGPOPUP_H
