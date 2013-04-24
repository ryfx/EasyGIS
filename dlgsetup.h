#ifndef DLGSETUP_H
#define DLGSETUP_H

#include <QDialog>
#include <QtGui>
#include "maptilesprovider.h"
#include "widget.h"
#include <QFileDialog>

namespace Ui {
class DlgSetup;
}

class DlgSetup : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgSetup(QWidget *parent = 0);
    ~DlgSetup();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    
private slots:
    void on_exitPushButton_clicked();

    void on_quxiaoPushButton_clicked();

    void on_quedingPushButton_clicked();

    void on_netDataToolButton_clicked();

    void on_localDataToolButton_clicked();

    void on_commonMapToolButton_clicked();

    void on_qMapToolButton_clicked();

    void on_browsePushButton_clicked();

    void on_startVoiceToolButton_clicked();

    void on_endVoiceToolButton_clicked();

private:
    Ui::DlgSetup *ui;
    QPoint dragPosition;

    bool m_useNetworkDataChecked;

    bool m_isSilenced;

    Widget* master;
};

#endif // DLGSETUP_H
