/*===========================================
 *滚动滑轮放大缩小地图时鼠标指针周围箭头的移动
 *模块重要性：一般
 *作者：杨飞
 *
 *--------------------------------------------
 *变更履历：
 *============================================*/

#include "zoomeffect.h"
#include "ui_zoomeffect.h"

ZoomEffect::ZoomEffect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZoomEffect)
{
    ui->setupUi(this);
    setLayout(ui->zoomLayout);
    setVisible(false);
}

ZoomEffect::~ZoomEffect()
{
    delete ui;
}
/*==================================
 *加载鼠标周围呈缩小放大状的4个箭头
 *==================================*/
void ZoomEffect::zoomInEffect()
{
    ui->label_1->setPixmap(QPixmap(":/images/1.png"));
    ui->label_2->setPixmap(QPixmap(":/images/2.png"));
    ui->label_3->setPixmap(QPixmap(":/images/3.png"));
    ui->label_4->setPixmap(QPixmap(":/images/4.png"));
    this->setStyleSheet("border-image:none");

    setVisible(true);

    QPropertyAnimation *animation1 = new QPropertyAnimation(ui->label_1, "geometry");
    animation1->setDuration(400);
    animation1->setStartValue(QRect(24, 18, 20, 20));
    animation1->setEndValue(QRect(0, 0, 20, 20));
    animation1->start();
    QPropertyAnimation *animation2 = new QPropertyAnimation(ui->label_2, "geometry");
    animation2->setDuration(400);
    animation2->setStartValue(QRect(72, 18, 20, 20));
    animation2->setEndValue(QRect(96, 0, 20, 20));
    animation2->start();
    QPropertyAnimation *animation3 = new QPropertyAnimation(ui->label_3, "geometry");
    animation3->setDuration(400);
    animation3->setStartValue(QRect(72, 54, 20, 20));
    animation3->setEndValue(QRect(96, 72, 20, 20));
    animation3->start();
    QPropertyAnimation *animation4 = new QPropertyAnimation(ui->label_4, "geometry");
    animation4->setDuration(400);
    animation4->setStartValue(QRect(24, 54, 20, 20));
    animation4->setEndValue(QRect(0, 72, 20, 20));
    animation4->start();
}

void ZoomEffect::zoomOutEffect()
{
    ui->label_1->setPixmap(QPixmap(":/images/5.png"));
    ui->label_2->setPixmap(QPixmap(":/images/6.png"));
    ui->label_3->setPixmap(QPixmap(":/images/7.png"));
    ui->label_4->setPixmap(QPixmap(":/images/8.png"));
    //this->setStyleSheet("border-image:none");

    setVisible(true);

    QPropertyAnimation *animation1 = new QPropertyAnimation(ui->label_1, "geometry");
    animation1->setDuration(400);
    animation1->setStartValue(QRect(0, 0, 20, 20));
    animation1->setEndValue(QRect(24, 18, 20, 20));
    animation1->start();
    QPropertyAnimation *animation2 = new QPropertyAnimation(ui->label_2, "geometry");
    animation2->setDuration(400);
    animation2->setStartValue(QRect(96, 0, 20, 20));
    animation2->setEndValue(QRect(72, 18, 20, 20));
    animation2->start();
    QPropertyAnimation *animation3 = new QPropertyAnimation(ui->label_3, "geometry");
    animation3->setDuration(400);
    animation3->setStartValue(QRect(96, 72, 20, 20));
    animation3->setEndValue(QRect(72, 54, 20, 20));
    animation3->start();
    QPropertyAnimation *animation4 = new QPropertyAnimation(ui->label_4, "geometry");
    animation4->setDuration(400);
    animation4->setStartValue(QRect(0, 72, 20, 20));
    animation4->setEndValue(QRect(24, 54, 20, 20));
    animation4->start();

}


