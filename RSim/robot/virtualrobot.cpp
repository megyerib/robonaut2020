#include "virtualrobot.h"

void VirtualRobot::InitRefreshTimer(double interval)
{
    refreshTimer = new QTimer();
    refreshTimer->setInterval(int(interval*1000));
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    refreshTimer->start();
}
