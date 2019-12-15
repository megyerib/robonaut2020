#pragma once

#include "Starter.h"
#include "Remote.h"

#include "Steering.h"
#include "Traction.h"
#include "TrackDetector.h"
#include "Distance.h"

#include "Timepiece.h"
#include "Trace.h"
#include "WaitDistance.h"

typedef enum
{
    Wait = 0,
    Straight,
    Decelerate,
    Turn,
    Accelerate,
    Follow,
    Stop
} QualiState;

class Car
{
private:
    Starter*        radio;
    Remote*         remote;
    Steering*       wheels;
    Traction*       motor;
    TrackDetector*  lineSensor;
    Distance*       distance;
    Timepiece*      clock;
    Trace*          trace;
    WaitDistance*   delayDistance;

    QualiState      state;
    QualiState      recover;

    void WaitState();

public:
    static Car* GetInstance();
    void StateMachine();
    void SetSteeringMode(SteeringMode mode);

private:
    Car();
    void CheckDeadmanSwitch();
    void FollowStateMachine();
};
