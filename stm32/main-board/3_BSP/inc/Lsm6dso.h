#pragma once

#include "Inert_I2c.h"

class LSM6DSO
{
private:
    INERT_I2C* i2c;

public:
    LSM6DSO();

    void Init();
    void Process();

private:
    void OffsetCalibration();
};
