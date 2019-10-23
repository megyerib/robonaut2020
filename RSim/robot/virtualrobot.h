#pragma once

#include "simulation/display.h"
#include <QTimer>

class VirtualRobot : public QObject
{
    Q_OBJECT

protected:
    Display* display;
    QTimer*  refreshTimer;

    void InitRefreshTimer(double interval);

public:
    virtual ~VirtualRobot() = default;

    virtual void Draw() = 0;

private slots:
    virtual void Refresh() = 0;
};
