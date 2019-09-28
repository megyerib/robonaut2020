#ifndef SIMSETTING_H
#define SIMSETTING_H

#include "QImage"
#include "robotproxy.h"
#include "virtualrobot.h"

class SimSetting
{
public:
    QImage trackImg;
    int pixelPerMeter;
    RobotProxy* robot;
    VirtualRobot* vrobot;

    SimSetting(const char* imgPath,
               int pixelPerMeter,
               RobotProxy* robot,
               VirtualRobot* vrobot);
};

#endif // SIMSETTING_H
