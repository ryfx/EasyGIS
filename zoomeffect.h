#ifndef ZOOMEFFECT_H
#define ZOOMEFFECT_H

#include <QWidget>
#include <QtGui>

namespace Ui {
class ZoomEffect;
}

class ZoomEffect : public QWidget
{
    Q_OBJECT
    
public:
    explicit ZoomEffect(QWidget *parent = 0);
    ~ZoomEffect();

    void zoomInEffect();
    void zoomOutEffect();

private:
    Ui::ZoomEffect *ui;

};

#endif // ZOOMEFFECT_H
