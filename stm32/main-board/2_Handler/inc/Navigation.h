#pragma once

#include "Lsm6dso.h"

#define NAVIGATION_SAMPLING_CYCLE   5 /* ms */

typedef enum
{
    STM = 0,
    GY91
} InertialSensor;

class Navigation
{
private:
    LSM6DSO* inert;

public:
    static Navigation* GetInstance();

    void Process();

private:
    Navigation();
};
