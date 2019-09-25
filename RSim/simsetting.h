#ifndef SIMSETTING_H
#define SIMSETTING_H

#include "QImage"

class SimSetting
{
public:
    QImage trackImg;
    int pixelPerMeter;

    SimSetting(const char* imgPath, int pixelPerMeter);
};

#endif // SIMSETTING_H
