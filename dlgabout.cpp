/*======================================
 *“关于道听图说”模块
 *功能 ： 新弹出的一个对话框，显示关于“道听图说”软件的信息
 *模块重要性 ： 一般
 *
 *作者 ： 杨飞
 *
 *--------------------------------------
 *变跟履历：
 *
 *======================================*/
#include "dlgabout.h"
#include "ui_dlgabout.h"

DlgAbout::DlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAbout)
{
    ui->setupUi(this);
}

DlgAbout::~DlgAbout()
{
    delete ui;
}

void DlgAbout::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DlgAbout::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void DlgAbout::on_quitPushButton_clicked()
{
    this->close();
}


