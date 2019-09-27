#pragma once

#include "navimath2d.h"
#include <QPainter>

class RobotProxy
{
    // No variables
    // This abstract class can only contain interface functions.

public:
    virtual ~RobotProxy() = default;

    virtual void SetSpeed(double speed) = 0;
    virtual void SetSteering(double angle) = 0;

    virtual void Draw(QWidget& w, CartesianCS* targetCS) = 0; // For simulator only
};
