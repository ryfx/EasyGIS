/*======================================
 *“设置”模块
 *功能 ： 新弹出的一个对话框，对“道听图说”软件进行基本设置
 *模块重要性 ： 重要
 *
 *作者 ： 杨飞，刘梓懿
 *
 *--------------------------------------
 *变跟履历：
 *
 *======================================*/
#include "dlgsetup.h"
#include "ui_dlgsetup.h"

DlgSetup::DlgSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetup)
{
    ui->setupUi(this);

    master = (Widget *)parentWidget();
    if(master == NULL)
    {
        QMessageBox::critical(this, tr("游戏结束"), tr("重新来过"), QMessageBox::Ok);
        QCoreApplication::quit();
    }

    //读取各种设置
    if(master->getMapFrame()->provider().getDataSource() == MapTilesProvider::NetworkData)
    {
        on_netDataToolButton_clicked();
    }
    else
    {
        on_localDataToolButton_clicked();
    }

    if(master->getRouteWidget()->sm->isSilenced())
    {
        on_endVoiceToolButton_clicked();
    }
    else
    {
        on_startVoiceToolButton_clicked();
    }


}

DlgSetup::~DlgSetup()
{
    delete ui;
}

void DlgSetup::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DlgSetup::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}


void DlgSetup::on_exitPushButton_clicked()
{
    this->reject();
}

void DlgSetup::on_quxiaoPushButton_clicked()
{
    this->reject();
}

void DlgSetup::on_quedingPushButton_clicked()
{
    if(m_isSilenced)
    {
        master->getRouteWidget()->sm->setSilenced(true);
    }
    else
    {
        master->getRouteWidget()->sm->setSilenced(false);
    }


    //判断地址栏输入的地址是否合法！
    if(m_useNetworkDataChecked)
    {
        //TODO: isValid()有点问题
        if(QUrl(ui->dizhiLineEdit->text()).isValid())
        {
            master->getMapFrame()->provider().setDataSource(MapTilesProvider::NetworkData);
            master->getMapFrame()->provider().setServerAddress(ui->dizhiLineEdit->text().trimmed());
        }
        else
        {
            QMessageBox::critical(this,
                                  tr("服务器地址不合法"),
                                  tr("您输入的地图数据服务器地址不合法，请返回重新输入！"),
                                  QMessageBox::Ok);
            return;
        }
    }
    else
    {
        master->getMapFrame()->provider().setDataSource(MapTilesProvider::LocalData);
        master->getMapFrame()->provider().setServerAddress(QDir::fromNativeSeparators(ui->dizhiLineEdit->text()).trimmed());
    }
    accept();
}

//网络地图跟离线地图radiobutton的实现
void DlgSetup::on_netDataToolButton_clicked()
{
    m_useNetworkDataChecked = true;
    ui->netDataToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
    ui->localDataToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
    ui->addressLabel->setStyleSheet("border-image:url(:/images/serverAddr.png)");
    if(master->getMapFrame()->provider().getDataSource() == MapTilesProvider::NetworkData)
    {
        ui->dizhiLineEdit->setText(master->getMapFrame()->provider().getServerAddress());
    }
    else
    {
        ui->dizhiLineEdit->setText("http://");
    }
    ui->browsePushButton->hide();
}

void DlgSetup::on_localDataToolButton_clicked()
{
    m_useNetworkDataChecked = false;
    ui->localDataToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
    ui->netDataToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
    ui->addressLabel->setStyleSheet("border-image:url(:/images/localAddr.png)");
    if(master->getMapFrame()->provider().getDataSource() == MapTilesProvider::LocalData)
    {
        ui->dizhiLineEdit->setText(master->getMapFrame()->provider().getServerAddress());
    }
    else
    {
        ui->dizhiLineEdit->setText("");
    }
    ui->browsePushButton->show();
}

//Q版地图跟普通地图radiobutton的实现
void DlgSetup::on_commonMapToolButton_clicked()
{
    ui->commonMapToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
    ui->qMapToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
}

void DlgSetup::on_qMapToolButton_clicked()
{
    ui->commonMapToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
    ui->qMapToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
}

//开启语音跟关闭语音radiobutton的实现
void DlgSetup::on_startVoiceToolButton_clicked()
{
    m_isSilenced = false;
    ui->startVoiceToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
    ui->endVoiceToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
}

void DlgSetup::on_endVoiceToolButton_clicked()
{
    m_isSilenced = true;
    ui->startVoiceToolButton->setIcon(QPixmap(":/images/radiobutton1.png"));
    ui->endVoiceToolButton->setIcon(QPixmap(":/images/radiobutton2.png"));
}

//获取本地地图地址
void DlgSetup::on_browsePushButton_clicked()
{
    //QFileDialog::HideNameFilterDetails

    QString dir = QFileDialog::getExistingDirectory(this, tr("选择离线地图文件夹"),
                                                    QCoreApplication::applicationDirPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    ui->dizhiLineEdit->setText(QDir::toNativeSeparators(dir));
}
