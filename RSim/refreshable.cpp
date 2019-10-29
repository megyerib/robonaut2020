#include "refreshable.h"

void Refreshable::InitRefreshTimer(double interval /* [s] */)
{
    refreshTimer = new QTimer();
    refreshTimer->setInterval(int(interval*1000));
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    refreshTimer->start();
}

Refreshable::~Refreshable()
{
    if (refreshTimer != nullptr)
        delete refreshTimer;
}
