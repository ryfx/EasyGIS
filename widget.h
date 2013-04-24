#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include "maptilesframe.h"

#include "locationwidget.h"
#include "routewidget.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    MapTilesFrame* getMapFrame();
    RouteWidget* getRouteWidget();

    
private:
    Ui::Widget *ui;
    QPoint dragPosition;
    bool xunLu;
    bool dingWei;
    bool chaZhao;
    bool m_isSilenced;
    bool isshowShortestRoute;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
private slots:
    void on_exitPushButton_clicked();
    void on_enlargeToolButton_clicked();
    void on_shrinkToolButton_clicked();
    void on_aboutDlgToolButton_clicked();
    void on_searchToolButton_clicked();
    void on_locateToolButton_clicked();
    void on_xunluToolButton_clicked();
    void on_qidianToolButton_clicked();
    void on_zhongdianToolButton_clicked();
    void on_volumeToolButton_clicked();
    void on_setupDlgToolButton_clicked();
    void on_pushButton_clicked();
    void on_quedingToolButton_clicked();
    void on_zizhuxunluToolButton_clicked();
    void on_sousuoToolButton_clicked();
    void on_recommendRouteToolButton_clicked();
    void on_beautifulRouteToolButton_clicked();
    void on_quxiaoButton_clicked();
};

#endif // WIDGET_H
