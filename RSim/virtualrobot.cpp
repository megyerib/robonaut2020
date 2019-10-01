#include "virtualrobot.h"

void VirtualRobot::PassTrackParameters(QWidget& w, QImage& bgImg, CartesianCS* bgCS, CartesianCS* windowCS)
{
    this->bgWidget = &w;
    this->bgImg    = &bgImg;
    this->bgCS     = bgCS;
    this->windowCS = windowCS;
}
