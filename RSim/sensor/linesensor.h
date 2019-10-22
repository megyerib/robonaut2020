#ifndef LINESENSOR_H
#define LINESENSOR_H

#include "navimath2d.h"
#include <QImage>

class LineSensor
{
    QImage* bgImg;
    CartesianCS* bgCS;

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

private:
    double lineLen(QLine& l);
    bool rangeValid(QPoint& p);

public:
    CartesianLoc* startLoc;
    CartesianLoc* endLoc;

    QPoint startPx;
    QPoint endPx;
};

#endif // LINESENSOR_H
