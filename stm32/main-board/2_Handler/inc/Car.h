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
#include "StringQueue.h"
#include "WaitDistance.h"
#include "WaitTime.h"
#include "Map.h"
#include "Pid_Controller.h"

#define USE_RADIO_STARTER      (0U)    /* 0 = with radio, 1 = can start without radio */
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
    sp_Chase,
    sp_PrepareForLaps,
    sp_Lap1,
    sp_Lap2,
    sp_Lap3,
    sp_Stop
} RaceState;

typedef enum
{
    rs_Straight,
    rs_Decelerate,
    rs_Turn,
    rs_Accelerate,
} RoadSegment_SM;

typedef enum
{
    PrepareForLaneChanging = 0,
    Y_part1,
    Y_part2,
    LeaveLine,
    SearchLineOnRight,
    LineFound,
    NoLineFound
} LineSwitch_SM;

typedef enum
{
    PrepareForReversing = 0,
    Reversing,
    JunctionFound
} Reversing_SM;

typedef struct
{
    RaceState    state;
    float        speed;
    float        dist_travelled;
    TrackType    track;
    Position     position;
    float        front_distance;
    SteeringMode wheel_mode;
    float        targetSpeed;
    float        lineFollow_Front;
    float        lineFollow_Rear;
    float        sensorServoAngle;
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
    Transmitter*    trace;
    WaitDistance*   delayDistance;
    //WaitTime*       delayTime;
    Navigation*     navigation;

    Map*            map;
    TurnType        nextTurn;

    CarProperties   carProp;
    RaceState       recoverState;

    bool            speedRunStarted;
    RoadSegment_SM  roadSegment;
    uint8_t         segmentCounter;
    bool            lapFinished;
    uint8_t         followLapCnt;
    bool            tryOvertake;

    TrackType       exitType;
    LineSwitch_SM   switchState;
    Reversing_SM    reversingState;

    Pid_Controller* dist_ctrl;

    void WaitState();

public:
    static Car* GetInstance();
    void StateMachine();
    void SetSteeringMode(SteeringMode mode);
    void Reset_To_State(RaceState state);
    void Reset_To_FailedOvertake();

private:
    Car();

    void BasicLabyrinth_StateMachine();
    void BaseRace_StateMachine();
    void RoadSegment_StateMachine();
    void Follow_StateMachine();
    void Minimal_StateMachine();
    void Race_StateMachine();

    void Maneuver_Reverse();
    void Maneuver_ChangeLane();
    void Maneuver_Overtake();

    void CheckDeadmanSwitch();
    void UpdateProperties();
    void Actuate();

    void ChangeState(RaceState const State);
    void ChangeRoadSegment(RoadSegment_SM const Segment);

    LineDirection SelectLineDirection(TurnType const turn);
    int prescaler;
};
