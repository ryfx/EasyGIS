/*===================================================
 *主窗口模块
 *功能说明：创建主窗口，显示窗口中各功能按钮以及功能面板切换效果
 *模块重要性：重要
 *作者：杨飞
 *
 *---------------------------------------------------
 *变更履历：
 *2013.3.19 增加拖动主界面窗口鼠标指针呈收放手形状
 *==================================================*/
#include "widget.h"
#include "ui_widget.h"
#include "dlgabout.h"
#include "dlgsetup.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    dingWei = true;
    chaZhao = true;
    m_isSilenced = false;
    isshowShortestRoute = true;
    ui->xunluWidget->setVisible(false);
    ui->locatingLabel->setVisible(false);
}

Widget::~Widget()
{
    delete ui;
}

MapTilesFrame *Widget::getMapFrame()
{
    return ui->mapFrame;
}

RouteWidget *Widget::getRouteWidget()
{
    return ui->routeWidget;
}

//使主窗口可以拖拽
void Widget::mousePressEvent(QMouseEvent *event)
{
    ui->windowWidget->setCursor(Qt::ClosedHandCursor);
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
void Widget::mouseReleaseEvent(QMouseEvent *)
{
    qDebug() << "mouseRelease called";
    ui->windowWidget->setCursor(Qt::OpenHandCursor);
}

//关闭系统
void Widget::on_exitPushButton_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry"); //关闭软件的卷轴闭合效果
    animation->setDuration(1000);
    animation->setStartValue(QRect(0, 0, 1210, 770));
    animation->setEndValue(QRect(605, 0, 0, 770));
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start();
    //让程序等待1秒关闭，实现关闭卷轴效果
    QTime t;
    t.start();
    while(t.elapsed()<1000)
        QCoreApplication::processEvents();
    QCoreApplication::quit();
}

//放大地图按钮
void Widget::on_enlargeToolButton_clicked()
{
    ui->mapFrame->zoomIn();
}

//缩小地图按钮
void Widget::on_shrinkToolButton_clicked()
{
    ui->mapFrame->zoomOut();
}

//关于“道听途说”按钮
void Widget::on_aboutDlgToolButton_clicked()
{
    DlgAbout *dlg = new DlgAbout(this);
    dlg->setWindowFlags(Qt::FramelessWindowHint);
    dlg->setAttribute(Qt::WA_TranslucentBackground);
    dlg->exec();
}

/*=======================================================================================
 *当点击“查找”按钮时，如果在寻路功能面板，查找功能的sousuoLineEdiit跟sousuoToolButton就会自左向右出现，
 *形成一种将寻路功能面板推出去的效果
 *
 =======================================================================================*/
void Widget::on_searchToolButton_clicked()
{
    chaZhao = true;
    if(xunLu == true)
    {
        //搜索面板向右移入
        QPropertyAnimation *animation = new QPropertyAnimation(ui->sousuoLineEdit, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(875, 300, 0, 40));
        animation->setEndValue(QRect(875, 300, 170, 40));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->sousuoToolButton, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(1045, 295, 0, 50));
        animation1->setEndValue(QRect(1045, 295, 70, 50));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();

        //寻路面板向右移出
        ui->xunluWidget->setVisible(true);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 220, 480));
        animation4->setEndValue(QRect(1100, 210, 0, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    else
    {
        //搜索面版向右移入
        QPropertyAnimation *animation = new QPropertyAnimation(ui->sousuoLineEdit, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(875, 300, 0, 40));
        animation->setEndValue(QRect(875, 300, 170, 40));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->sousuoToolButton, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(1045, 295, 0, 50));
        animation1->setEndValue(QRect(1045, 295, 70, 50));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();
    }
    xunLu = false;
}


void Widget::on_locateToolButton_clicked()
{      
    if(xunLu == true)
    {
        //定位面板向右移入
        QPropertyAnimation *animation = new QPropertyAnimation(ui->wozaiLabel, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(860, 215, 0, 70));
        animation->setEndValue(QRect(860, 215, 70, 70));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->locationLabel, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(930, 230, 0, 40));
        animation1->setEndValue(QRect(930, 230, 170, 40));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();


        //寻路面板向右移出
        ui->xunluWidget->setVisible(true);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 220, 480));
        animation4->setEndValue(QRect(1100, 210, 0, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    else
    {//定位面板向右移入
        QPropertyAnimation *animation = new QPropertyAnimation(ui->wozaiLabel, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(860, 215, 0, 70));
        animation->setEndValue(QRect(860, 215, 60, 70));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->locationLabel, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(930, 230, 0, 40));
        animation1->setEndValue(QRect(930, 230, 170, 40));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();
    }
    dingWei = true;
    xunLu = false;

    ui->locatingLabel->setVisible(true);
    QTime t;
    t.start();
    while(t.elapsed()<2000)
        QCoreApplication::processEvents();
    ui->locatingLabel->setVisible(false);

    if(ui->routeWidget->userLocationWidget)
    {
        delete ui->routeWidget->userLocationWidget;
        ui->routeWidget->userLocationWidget = NULL;
    }
    ui->routeWidget->userLocationWidget = new LocationWidget(ui->mapFrame);
    connect(ui->routeWidget->userLocationWidget,
            SIGNAL(serverCoordinateChanged(QPoint,QPoint)),
            ui->routeWidget->sm,
            SLOT(userLocationChangedSlot(QPoint)));
    ui->routeWidget->userLocationWidget->setRandomPos();


    ui->locationLabel->setText(ui->routeWidget->findNearestSpotName(ui->routeWidget->userLocationWidget->getServerCoordinate()) + " 附近");
}

void Widget::on_xunluToolButton_clicked()
{
    if (dingWei==true&&chaZhao==true)
    {
        //搜索面板向右移出
        QPropertyAnimation *animation = new QPropertyAnimation(ui->sousuoLineEdit, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(875, 300, 170, 40));
        animation->setEndValue(QRect(1100, 300, 0, 40));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->sousuoToolButton, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(1045, 295, 70, 50));
        animation1->setEndValue(QRect(1100, 295, 0, 50));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();

        //定位面板向右移出
        QPropertyAnimation *animation2 = new QPropertyAnimation(ui->wozaiLabel, "geometry");
        animation2->setDuration(1000);
        animation2->setStartValue(QRect(860, 215, 70, 70));
        animation2->setEndValue(QRect(1100, 215, 0, 70));
        animation2->setEasingCurve(QEasingCurve::InOutQuad);
        animation2->start();

        QPropertyAnimation *animation3 = new QPropertyAnimation(ui->locationLabel, "geometry");
        animation3->setDuration(1000);
        animation3->setStartValue(QRect(930, 230, 170, 40));
        animation3->setEndValue(QRect(1100, 230, 0, 40));
        animation3->setEasingCurve(QEasingCurve::InOutQuad);
        animation3->start();

        //寻路面板向右移入
        ui->xunluWidget->setVisible(true);
        ui->zizhuxunluWidget->setVisible(false);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 0, 480));
        animation4->setEndValue(QRect(880, 210, 220, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    else if(dingWei == false && chaZhao == true)
    {
        //搜索面板向右移出
        QPropertyAnimation *animation = new QPropertyAnimation(ui->sousuoLineEdit, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(875, 300, 170, 40));
        animation->setEndValue(QRect(1100, 300, 0, 40));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();

        QPropertyAnimation *animation1 = new QPropertyAnimation(ui->sousuoToolButton, "geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(1045, 295, 70, 50));
        animation1->setEndValue(QRect(1100, 295, 0, 50));
        animation1->setEasingCurve(QEasingCurve::InOutQuad);
        animation1->start();
        //寻路面板向右移入
        ui->xunluWidget->setVisible(true);
        ui->zizhuxunluWidget->setVisible(false);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 0, 480));
        animation4->setEndValue(QRect(880, 210, 220, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    else if(dingWei == true && chaZhao == false)
    {
        //定位面板向右移出
        QPropertyAnimation *animation2 = new QPropertyAnimation(ui->wozaiLabel, "geometry");
        animation2->setDuration(1000);
        animation2->setStartValue(QRect(860, 215, 70, 70));
        animation2->setEndValue(QRect(1100, 215, 0, 70));
        animation2->setEasingCurve(QEasingCurve::InOutQuad);
        animation2->start();

        QPropertyAnimation *animation3 = new QPropertyAnimation(ui->locationLabel, "geometry");
        animation3->setDuration(1000);
        animation3->setStartValue(QRect(930, 230, 170, 40));
        animation3->setEndValue(QRect(1100, 230, 0, 40));
        animation3->setEasingCurve(QEasingCurve::InOutQuad);
        animation3->start();

        //寻路面板向右移入
        ui->xunluWidget->setVisible(true);
        ui->zizhuxunluWidget->setVisible(false);
        QPropertyAnimation *animation4 = new QPropertyAnimation(ui->xunluWidget, "geometry");
        animation4->setDuration(1000);
        animation4->setStartValue(QRect(880, 210, 0, 480));
        animation4->setEndValue(QRect(880, 210, 220, 480));
        animation4->setEasingCurve(QEasingCurve::InOutQuad);
        animation4->start();
    }
    xunLu = true;
    dingWei = false;
    chaZhao = false;

}

void Widget::on_qidianToolButton_clicked()
{
    QCursor my(QPixmap(":/images/start.png"));//更改鼠标图标为绿色起点图标
    ui->routeWidget->setCursor(my);
    ui->routeWidget->raise();
    ui->routeWidget->isPlacingStartLocation = true;
}

void Widget::on_zhongdianToolButton_clicked()
{

    QCursor my(QPixmap(":/images/end.png"));//更改鼠标图标为红色终点图标
    ui->routeWidget->setCursor(my);
    ui->routeWidget->raise();
    ui->routeWidget->isPlacingEndLocation = true;

}

void Widget::on_volumeToolButton_clicked()
{
    if(m_isSilenced == false)
    {
        ui->routeWidget->sm->setSilenced(true);
        ui->volumeToolButton->setIcon(QIcon(":/images/jingyin.png"));
        m_isSilenced = true;
    }
    else
    {
        ui->routeWidget->sm->setSilenced(false);
        ui->volumeToolButton->setIcon(QIcon(":/images/shengyin.png"));
        m_isSilenced = false;
    }
}

/*创建设置对话框
 *将背景色设为透明
 */
void Widget::on_setupDlgToolButton_clicked()
{
    DlgSetup *dlgset = new DlgSetup(this);
    dlgset->setWindowFlags(Qt::FramelessWindowHint);
    dlgset->setAttribute(Qt::WA_TranslucentBackground);
    dlgset->exec();
}


void Widget::on_pushButton_clicked()
{
    showMinimized();
}


void Widget::on_quedingToolButton_clicked()
{
    bool ok = false;
    QPoint startPos(ui->routeWidget->findSpotLocation(ui->qidianLineEdit->text(), &ok));

    if(!ok)
        return;

    QPoint endPos(ui->routeWidget->findSpotLocation(ui->zhongdianLineEdit->text(), &ok));

    if(!ok)
        return;

    if(ui->routeWidget->startWidget != NULL)
    {
        delete ui->routeWidget->startWidget;
        ui->routeWidget->startWidget = NULL;
    }
    ui->routeWidget->startWidget = new LocationWidget(ui->mapFrame);
    ui->routeWidget->startWidget->setPixmap(QPixmap(":/images/start.png"));
    ui->routeWidget->startWidget->setServerCoordinate(startPos);

    if(ui->routeWidget->endWidget != NULL)
    {
        delete ui->routeWidget->endWidget;
        ui->routeWidget->endWidget = NULL;
    }
    ui->routeWidget->endWidget = new LocationWidget(ui->mapFrame);
    ui->routeWidget->endWidget->setPixmap(QPixmap(":/images/end.png"));
    ui->routeWidget->endWidget->setServerCoordinate(endPos);

    ui->routeWidget->locationSet(false);
}

void Widget::on_zizhuxunluToolButton_clicked()
{
    if(isshowShortestRoute == true)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(ui->zizhuxunluWidget, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(0, 210, 0, 280));
        animation->setEndValue(QRect(0, 210, 220, 280));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();
        isshowShortestRoute = false;
    }
    else
    {
        QPropertyAnimation *animation = new QPropertyAnimation(ui->zizhuxunluWidget, "geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(0, 210, 220, 280));
        animation->setEndValue(QRect(0, 210, 0, 280));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->start();
        isshowShortestRoute = true;
    }

    ui->zizhuxunluWidget->setVisible(true);
}

void Widget::on_sousuoToolButton_clicked()
{
    if(ui->routeWidget->searchResultWidget)
    {
        delete ui->routeWidget->searchResultWidget;
        ui->routeWidget->searchResultWidget = NULL;
    }

    if(ui->sousuoLineEdit->text().isEmpty())
    {
        return;
    }

    bool isFound = false;
    QPoint pos = ui->routeWidget->findSpotLocation(ui->sousuoLineEdit->text(), &isFound);

    if(!isFound)
    {
        return;
    }

    ui->routeWidget->searchResultWidget = new LocationWidget(ui->mapFrame, pos);

}

void Widget::on_recommendRouteToolButton_clicked()
{
    static bool isSet = false;
    if(!isSet)
    {
        isSet = true;
        ui->mapFrame->provider().setMapType("/SuggestedRouteMap/");
        //ui->recommendRouteToolButton->setText(tr("取消设置"));
    }
    else
    {
        isSet = false;
        ui->mapFrame->provider().setMapType("/NormalMap/");
    }
}

void Widget::on_beautifulRouteToolButton_clicked()
{
    static bool isSet = false;
    if(!isSet)
    {
        isSet = true;
        ui->mapFrame->provider().setMapType("/ScenicRouteMap/");
        //ui->recommendRouteToolButton->setText(tr("取消设置"));
    }
    else
    {
        isSet = false;
        ui->mapFrame->provider().setMapType("/NormalMap/");
    }
}

void Widget::on_quxiaoButton_clicked()
{
    ui->routeWidget->isRoute = false;
    ui->routeWidget->m_isRouteByCoor = false;
    ui->routeWidget->update();
    if(ui->routeWidget->startWidget != NULL)
    {
        delete ui->routeWidget->startWidget;
        ui->routeWidget->startWidget = NULL;
    }
    if(ui->routeWidget->endWidget != NULL)
    {
        delete ui->routeWidget->endWidget;
        ui->routeWidget->endWidget = NULL;
    }
    if(ui->routeWidget->userLocationWidget != NULL)
    {
        delete ui->routeWidget->userLocationWidget;
        ui->routeWidget->userLocationWidget = NULL;
    }
    if(ui->routeWidget->searchResultWidget != NULL)
    {
        delete ui->routeWidget->searchResultWidget;
        ui->routeWidget->searchResultWidget = NULL;
    }
}
