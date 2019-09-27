#include "simsetting.h"

SimSetting::SimSetting(const char* imgPath, int pixelPerMeter, RobotProxy *robot) :
    trackImg(imgPath),
    pixelPerMeter(pixelPerMeter),
    robot(robot)
{

}
