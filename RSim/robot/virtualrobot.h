#pragma once

#include "simulation/display.h"
#include "refreshable.h"

class VirtualRobot : public Refreshable
{
protected:
    Display* display;
    QTimer*  refreshTimer;

public:
    virtual ~VirtualRobot() = default;

    virtual void Draw() = 0;
    virtual CartesianPos GetRealPosition() = 0;
};
