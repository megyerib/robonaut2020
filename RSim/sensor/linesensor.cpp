#include "linesensor.h"
#include "math.h"

LineSensor::LineSensor
(
    CartesianLoc* startLoc,
    CartesianLoc* endLoc,
    QImage* bgImg,
    CartesianCS* bgCS
) :
    bgImg(bgImg),
    bgCS(bgCS),
    startLoc(startLoc),
    endLoc(endLoc)
{
    double x_diff = endLoc->x() - startLoc->x();
    double y_diff = endLoc->y() - startLoc->y();

    sensorLength = sqrt(x_diff * x_diff + y_diff * y_diff);
}

void LineSensor::calcPoints()
{
    /*CartesianLoc sensorStart = *startLoc; // alpha doesn't change
    sensorStart.TransformTo(bgCS);
    startPx.setX(int(sensorStart.x()));
    startPx.setY(int(sensorStart.y()));

    CartesianLoc sensorEnd = *endLoc;
    sensorEnd.TransformTo(bgCS);
    endPx.setX(int(sensorEnd.x()));
    endPx.setY(int(sensorEnd.y()));*/
}

void LineSensor::getPixels()
{
    pixels.clear();

    // Steps
    double x_step, y_step;
    double x_pos, y_pos;
    QLine line(startPx, endPx);
    double len = lineLen(line);

    if (line.dx() == 0)
    {
        x_step = 0.0;
        y_step = 1.0;
    }
    else if (line.dy() == 0)
    {
        y_step = 0.0;
        x_step = 1.0;
    }
    else if (abs(line.dx()) > abs(line.dy()))
    {
        x_step = 1.0;
        y_step = abs(double(line.dy())/double(line.dx()));
    }
    else // (line.dy() >= line.dy())
    {
        y_step = 1.0;
        x_step = abs(double(line.dx())/double(line.dy()));
    }

    // Sign
    if (x_step * line.dx() < 0) // Signs don't match
    {
        x_step *= -1.0;
    }

    if (y_step * line.dy() < 0) // Signs don't match
    {
        y_step *= -1.0;
    }

    QPoint i_point(startPx);
    QLine i_line(startPx, startPx);

    x_pos = startPx.x();
    y_pos = startPx.y();

    while (lineLen(i_line) <= len)
    {
        int lightness = 255;

        if (rangeValid(i_point))
        {
            QColor c = bgImg->pixelColor(i_point);
            lightness = c.lightness();
        }

        pixels.push_back(lightness);

        x_pos += x_step;
        y_pos += y_step;

        i_point.setX(int(x_pos));
        i_point.setY(int(y_pos));
        i_line.setP2(i_point);
    }
}

double LineSensor::lineLen(QLine& l)
{
    return sqrt(double(l.dx())*l.dx()+l.dy()*l.dy());
}

bool LineSensor::rangeValid(QPoint& p)
{
    bool ret = true;

    if (p.x() < 0               ||
        p.x() >= bgImg->width() ||
        p.y() < 0               ||
        p.y() >= bgImg->height())
    {
        ret = false;
    }

    return ret;
}
