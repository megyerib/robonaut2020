#pragma once

#include <QWidget>
#include <QImage>
#include "navimath2d.h"
#include "display.h"

class Track
{
    Display& display;

public:
    QImage       bgImg;    // Background image
    int          bgRes;    // Resolution [pixel/m]
    CartesianCS* bgCs;

    double       width;    // [m]
    double       height;   // [m]

    Track(Display& d,
          const char* imgPath,
          int pixelPerMeter);

    void Draw();
};
