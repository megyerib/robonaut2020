#pragma once

#include "Lsm6dso.h"
#include "Encoder.h"

#define NAVIGATION_SAMPLING_CYCLE   5 /* ms */

typedef enum
{
    STM = 0,
    GY91
} InertialSensor;

typedef struct  // In NED coordination system.
{
    float n;
    float e;
    float psi;
} Position;

class Navigation
{
private:
    LSM6DSO* inert;
    Encoder* enc;

    Position position;
    float d_s;
    float d_psi;
    float prev_enc;
    float prev_angular;

public:
    static Navigation* GetInstance();

    void Process();
    void SetPosition(Position const pos, float const ds, float const dpsi);
    Position GetPosition();

private:
    Navigation();

    void Init();
    void Odometry();
    void CalcDeltaOrientation();
    void CalcDeltaDistance();
    float Integral_Trapez(const float a, const float b, const float fa, const float fb);
};
