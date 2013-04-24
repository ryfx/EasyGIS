#include "soundmanager.h"

SoundManager::SoundManager(QObject *parent) :
    QObject(parent)
{
    m_isSilenced = false;

    mapFrame = qobject_cast<MapTilesFrame *>(parent);
    routeWidget = mapFrame->findChild<RouteWidget *>();

    thread = NULL;

    assert(mapFrame != NULL);
    assert(routeWidget != NULL);

}

bool SoundManager::isSilenced() const
{
    return m_isSilenced;
}

void SoundManager::userLocationChangedSlot(QPoint point)
{

    QSqlQuery query;
    QString str,str1;
    QString mediaSourcePath;
    QPoint nearSpotLoca = routeWidget->findNearestSpotLocation(point);
    double distance = mapFrame->distance(point,nearSpotLoca);
    if( distance < 300 )
    {
        str.setNum(nearSpotLoca.x());
        str1.setNum(nearSpotLoca.y());
        query.exec("SELECT SoundName FROM interestNode WHERE CoordinateX="+str+" AND CoordinateY="+str1);
        query.next();

        if(query.isValid())
        {
            mediaSourcePath = mapFrame->provider().getServerAddress() + "/Sound/" + query.value(0).toString();

            if(thread)
            {
                delete thread;
                thread = NULL;
            }
            thread = new PlayThread(this, mediaSourcePath, isSilenced());
            thread->start();
        }

    }
    else
    {
        if(thread)
        {
            delete thread;
            thread = NULL;
        }
    }
    return ;
}

void SoundManager::setSilenced(bool isSilenced)
{
    m_isSilenced = isSilenced;

    if(thread)
    {
        //qDebug() << "setMuted: " << isSilenced;
        thread->audioOutput->setMuted(isSilenced);
    }

    emit silenceStatusChanged(isSilenced);
}



