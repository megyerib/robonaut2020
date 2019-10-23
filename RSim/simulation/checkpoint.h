#pragma once

#include <QObject>
#include "simulation/display.h"
#include "navimath2d.h"
#include "robot/virtualrobot.h"
#include "refreshable.h"

class Checkpoint : public Refreshable
{
    bool visited = false;

    Display& display;
    CartesianPos pos;
    VirtualRobot& robot;

public:
    Checkpoint(Display& display, CartesianPos& pos, VirtualRobot& robot);
    void Draw();

private:
    void CalculatePoints();
    void Refresh();
};
