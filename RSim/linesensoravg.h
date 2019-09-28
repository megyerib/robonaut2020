#ifndef LINESENSORAVG_H
#define LINESENSORAVG_H

#include "linesensor.h"

class LineSensorAvg : public LineSensor
{
public:
    LineSensorAvg(CartesianLoc* startLoc,
                  CartesianLoc* endLoc,
                  QImage* bgImg,
                  CartesianCS* bgCS);

    fp_t getLine();
};

#endif // LINESENSORAVG_H
