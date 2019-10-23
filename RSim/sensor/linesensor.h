#ifndef LINESENSOR_H
#define LINESENSOR_H

#include "navimath2d.h"
#include "simulation/track.h"
#include "simulation/display.h"

class LineSensor
{
    Display* display;
    Track*   track;

public:
    void Draw();

protected:
    double sensorLength; // [m]
    QVector<int> pixels;

    LineSensor
    (
        Display*      d,
        Track*        t,
        CartesianLoc* start,
        CartesianLoc* end
    );

    void getPixels();

private:
    void calcPoints();
    double lineLen(QLine& l);
    bool pointValid(QPoint& p);

    CartesianLoc* startLoc;
    CartesianLoc* endLoc;

    QPoint startPx;
    QPoint endPx;
};

#endif // LINESENSOR_H
