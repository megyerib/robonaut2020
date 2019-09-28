#ifndef LINESENSOR_H
#define LINESENSOR_H

#include "navimath2d.h"
#include <QImage>

class LineSensor
{
    CartesianLoc* startLoc;
    CartesianLoc* endLoc;

    QImage* bgImg;
    CartesianCS* bgCS;

    QPoint startPx;
    QPoint endPx;

protected:
    double sensorLength; // [m]
    QVector<int> pixels;

    LineSensor
    (
        CartesianLoc* startLoc,
        CartesianLoc* endLoc,
        QImage* bgImg,
        CartesianCS* bgCS
    );

    void calcPoints();
    void getPixels();
};

#endif // LINESENSOR_H
