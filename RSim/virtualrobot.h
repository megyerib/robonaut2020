#ifndef VIRTUALROBOT_H
#define VIRTUALROBOT_H

#include <QWidget>
#include "navimath2d.h"

class VirtualRobot
{
protected:
    QWidget* bgWidget;
    QImage* bgImg;
    CartesianCS* windowCS;
    CartesianCS* bgCS;

public:
    virtual ~VirtualRobot() = default;

    void PassTrackParameters(QWidget& w, QImage& bgImg, CartesianCS* bgCS, CartesianCS* windowCS);
    virtual void Draw() = 0;
    virtual void PostCfg() = 0; // TODO redesign!
};

#endif // VIRTUALROBOT_H
