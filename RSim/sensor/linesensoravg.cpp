#include "linesensoravg.h"

LineSensorAvg::LineSensorAvg(Display *d,
                             Track *t,
                             CartesianLoc *start,
                             CartesianLoc *end) :
    LineSensor(d, t, start, end)
{

}

double LineSensorAvg::getLine()
{
    double ret = 0;

    getPixels();

    int blackSum = 0; // Sum of black pixel coordinates
    int blackCnt = 0; // Count of black pixel coordinates

    for (int i = 0; i < pixels.count(); i++)
    {
        if (pixels[i] == 0)
        {
            blackSum += i;
            blackCnt += 1;
        }
    }

    if (blackCnt > 0)
    {
        ret = double(blackSum) / blackCnt;
        ret /= pixels.count();
        ret *= sensorLength;
        ret -= sensorLength / 2;
    }

    return ret;
}
