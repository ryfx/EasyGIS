#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QtGui>
#include <QtSql>
#include "routewidget.h"
#include "maptilesframe.h"
#include "playthread.h"
#include <assert.h>

class MapTileFrame;
class RouteWidget;

class SoundManager : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(bool isSilenced READ isSilenced WRITE setSilenced NOTIFY silenceStatusChanged)

public:
    explicit SoundManager(QObject *parent = 0);

    bool isSilenced() const;
    
private:

    bool m_isSilenced;

    MapTilesFrame* mapFrame;
    RouteWidget* routeWidget;
    PlayThread* thread;
signals:
    void silenceStatusChanged(bool isSilenced);

public slots:
    void userLocationChangedSlot(QPoint point);

    void setSilenced(bool isSilenced);

    
};

#endif // SOUNDMANAGER_H
