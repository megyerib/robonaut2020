#ifndef LINESENSORAVG_H
#define LINESENSORAVG_H

#include "linesensor.h"

class LineSensorAvg : public LineSensor
{
public:
    LineSensorAvg(Display*      d,
                  Track*        t,
                  CartesianLoc* start,
                  CartesianLoc* end);

    double getLine();
};

#endif // LINESENSORAVG_H
