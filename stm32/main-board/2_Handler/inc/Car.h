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
#include "Map.h"
#include "Pid_Controller.h"

#define USE_RADIO_STARTER      (1U)    /* 0 = with radio, 1 = can start without radio */
#define USE_DEADMAN_SWITCH     (1U)    /* 0 = no remote,  1 = starts/stops with remote */
#define USE_MINIMAL_STRATEGY   (1U)    /* 0 = algorithms, 1 = basic drive */

typedef enum
{
    la_Idle = 0,
    la_Straight,
    la_Junction,
    la_Turn,
    la_Reverse,
    la_Exit,
    la_End,     // For test
    sp_Wait,    // For test
    sp_Follow,
    sp_Overtake,
    sp_Straight,
    sp_Decelerate,
    sp_Turn,
    sp_Accelerate,
    sp_Stop
} RaceState;

typedef enum
{
    InvalidLap = 0,
    Follow_One,
    Follow_Two,
    Lap_One,
    Lap_Two,
    Lap_Three,
} Lap;

typedef enum
{
    LeaveLine = 0,
    SearchLine,
    LineFound,
    NoLineFound
} LineSwitch_SM;

typedef struct
{
    RaceState    state;
    float        speed;
    uint32_t     dist_travelled;
    TrackType    track;
    Position     position;
    float        front_distance;
    SteeringMode wheel_mode;
    float        targetSpeed;
    float        lineFollow_Front;
    float        lineFollow_Rear;
} CarProperties;

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

    Map*            map;
    TurnType        nextTurn;

    CarProperties   carProp;
    RaceState       recoverState;

    Lap             actLap;
    uint8_t         segmentCounter;

    LineSwitch_SM   switchState;

    Pid_Controller* dist_ctrl;

    void WaitState();

public:
    static Car* GetInstance();
    void StateMachine();
    void SetSteeringMode(SteeringMode mode);

private:
    Car();

    void CheckDeadmanSwitch();

    void BasicDrive_StateMachine();
    void Follow_StateMachine();
    void BasicLabyrinth_StateMachine();
    void Race_StateMachine();
    void Minimal_StateMachine();

    void Maneuver_Reverse();
    void Maneuver_ChangeLane();
    void Maneuver_Overtake();

    void UpdateProperties();
    void Actuate();

    LineDirection SelectLineDirection(TurnType const turn);
    int prescaler;
};
