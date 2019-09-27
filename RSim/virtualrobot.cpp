#include "virtualrobot.h"

void VirtualRobot::PassTrackParameters(QWidget& widget, QImage& img, CartesianCS* cs)
{
    this->bgWidget = &widget;
    this->bgImg    = &img;
    this->bgCS     =  cs;
}
