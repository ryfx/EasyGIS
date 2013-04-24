#ifndef DLGABOUT_H
#define DLGABOUT_H

#include <QDialog>
#include <QtGui>


namespace Ui {
class DlgAbout;
}

class DlgAbout : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgAbout(QWidget *parent = 0);
    ~DlgAbout();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    
private slots:
    void on_quitPushButton_clicked();

private:
    Ui::DlgAbout *ui;
    QPoint dragPosition;
};

#endif // DLGABOUT_H
