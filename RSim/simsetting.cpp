#include "simsetting.h"

SimSetting::SimSetting(const char* imgPath, int pixelPerMeter, RobotProxy *robot, VirtualRobot* vrobot) :
    trackImg(imgPath),
    pixelPerMeter(pixelPerMeter),
    robot(robot),
    vrobot(vrobot)
{

}
