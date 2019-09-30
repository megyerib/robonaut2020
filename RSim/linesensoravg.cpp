#include "linesensoravg.h"

LineSensorAvg::LineSensorAvg(CartesianLoc* startLoc,
                             CartesianLoc* endLoc,
                             QImage* bgImg,
                             CartesianCS* bgCS) :
    LineSensor(startLoc, endLoc, bgImg, bgCS)
{

}

double LineSensorAvg::getLine()
{
    double ret = 0;

    calcPoints();
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
