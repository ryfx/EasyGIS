#include "navigationimitation.h"

NavigationImitation::NavigationImitation(QObject *parent, LocationWidget *userPosition) :
    QObject(parent)
{
    mapFrame = parent->findChild<MapTilesFrame *>();

    assert(mapFrame != NULL);

    if(userPosition)
    {
        userLocationWidget = userPosition;
    }
    else
    {
        userLocationWidget = new LocationWidget(mapFrame);
    }
}

NavigationImitation::~NavigationImitation()
{
    if(userLocationWidget)
    {
        delete userLocationWidget;
        userLocationWidget = NULL;
    }
}

int NavigationImitation::speed() const
{
    return m_speed;
}

void NavigationImitation::setSpeed(int newSpeed)
{
    int oldSpeed = m_speed;
    m_speed = newSpeed;
    emit speedChanged(newSpeed, oldSpeed);
}
