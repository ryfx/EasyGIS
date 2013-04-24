#ifndef NAVIGATIONIMITATION_H
#define NAVIGATIONIMITATION_H

#include <QObject>
#include <QtGui>
#include "maptilesframe.h"
#include "locationwidget.h"

class NavigationImitation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
public:
    explicit NavigationImitation(QObject *parent = 0, LocationWidget *userPosition = 0);
    ~NavigationImitation();

    int speed() const;
    void setSpeed(int newSpeed);

private:
    LocationWidget *userLocationWidget;
    MapTilesFrame *mapFrame;

    int m_speed;
    
signals:
    void speedChanged(int newSpeed, int oldSpeed);
    
public slots:
    
};

#endif // NAVIGATIONIMITATION_H
