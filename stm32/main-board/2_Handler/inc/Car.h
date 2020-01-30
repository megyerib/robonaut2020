#pragma once

#include "Starter.h"
#include "Remote.h"

#include "Encoder.h"
#include "Steering.h"
#include "Traction.h"
#include "TrackDetector.h"
#include "Distance.h"
#include "Navigation.h"

#include "Timepiece.h"
#include "Trace.h"
#include "WaitDistance.h"
#include "WaitTime.h"

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
    Encoder*        encoder;
    Starter*        radio;
    Remote*         remote;
    Steering*       wheels;
    Traction*       motor;
    TrackDetector*  lineSensor;
    Distance*       distance;
    Timepiece*      clock;
    Trace*          trace;
    WaitDistance*   delayDistance;
    //WaitTime*       delayTime;
    Navigation*     navigation;

    QualiState      state;
    QualiState      recover;

    Pd_Controller*  dist_ctrl;

    void WaitState();

public:
    static Car* GetInstance();
    void StateMachine();
    void SetSteeringMode(SteeringMode mode);

private:
    Car();
    void CheckDeadmanSwitch();
    void FollowStateMachine();

    int prescaler;
};
