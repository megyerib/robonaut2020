#include "linesensor.h"

LineSensor::LineSensor
(
    CartesianLoc* startLoc,
    CartesianLoc* endLoc,
    QImage* bgImg,
    CartesianCS* bgCS
) :
    startLoc(startLoc),
    endLoc(endLoc),
    bgImg(bgImg),
    bgCS(bgCS)
{
    double x_diff = endLoc->GetX() - startLoc->GetX();
    double y_diff = endLoc->GetY() - startLoc->GetY();

    sensorLength = sqrt(x_diff * x_diff + y_diff * y_diff);
}

void LineSensor::calcPoints()
{
    CartesianLoc sensorStart = *startLoc;
    sensorStart.TransformTo(bgCS);
    startPx.setX(int(sensorStart.GetX()));
    startPx.setY(int(sensorStart.GetY()));

    CartesianLoc sensorEnd = *endLoc;
    sensorEnd.TransformTo(bgCS);
    endPx.setX(int(sensorEnd.GetX()));
    endPx.setY(int(sensorEnd.GetY()));
}

void LineSensor::getPixels()
{
    int x_incr = startPx.x() < endPx.x() ? 1 : -1;
    int y_incr = startPx.y() < endPx.y() ? 1 : -1;

    int x_diff = x_incr * (endPx.x() - startPx.x());
    int y_diff = y_incr * (endPx.y() - startPx.y());

    pixels.clear();

    if (x_diff > y_diff)
    {
        double prop = double(y_diff) / double(x_diff);
        double y = startPx.y();

        for (int x = startPx.x();
             x_incr * (endPx.x() - x) > 0;
              x += x_incr)
        {
            int lightness = bgImg->pixelColor(x, int(y)).lightness();
            y += prop;
            pixels.push_back(lightness);
        }
    }
    else
    {
        double prop = double(x_diff) / double(y_diff);
        double x = startPx.x();

        for (int y = startPx.y();
             y_incr * (endPx.y() - y) > 0;
              y += y_incr)
        {
            QColor c = bgImg->pixelColor(int(x), y);
            int lightness = c.lightness();
            x += prop;
            pixels.push_back(lightness);
        }
    }
}
