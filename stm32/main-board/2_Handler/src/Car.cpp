#include "Car.h"
#include "Trace.h"

#define MAZE_FORWARD_SPEED          ( 0.17f)  /*   % */ //!< Speed used in the labyrinth when the robot is over a Single line.
#define MAZE_FORWARD_SLOW_SPEED     ( 0.16f)  /*   % */
#define MAZE_REVERSE_SPEED          (-0.14f)  /*   % */ //!< Speed used for reverse maneuver.
#define MAZE_REVERSE_P              ( 20.0f)
#define MAZE_REVERSE_D              (200.0f)
#define MAZE_EXIT_WAIT_DIST         ( 0.90f)  /*   m */ //!< Distance the robot will make to leave the labyrinth, before searching for the speedrun line.
#define MAZE_EXIT_REV_WAIT_DIST_1   ( 0.30f)
#define MAZE_EXIT_REV_WAIT_DIST_2   ( 0.80f)
#define MAZE_EXIT_REV_WAIT_DIST_3   ( 0.80f)
#define MAZE_EXIT_REV_WAIT_DIST_4   ( 0.80f)
#define MAZE_EXIT_WHEEL_ANGLE       (-1.74f)  /* rad */ //!< The angles of the servos, while leaving the labyrinth.
#define MAZE_EXIT_DIST_LIMIT        ( 1.00f)  /*   m */ //!< The distance how many meters can the robot drive while searching for the speedrun line.
#define MAZE_EXIT_SPEED             ( 0.12f)  /*   % */
#define MAZE_TURN_WAIT_DIST         ( 0.60f)  /*   m */

#define CAR_FOLLOW_MAX_APPROX       ( 1.80f)
#define CAR_FOLLOW_DISTANCE         ( 0.70f)  /*   m */
#define CAR_FOLLOW_MIN_APPROX       ( 0.65f)  /*   m */
#define CAR_FOLLOW_MAX_SPEED        ( 1.80f)  /* m/s */
#define CAR_FOLLOW_REDUCED_SPEED    ( 1.65f)
#define CAR_FOLLOW_REV_SPEED        (-3.00f)  /* m/s */

#define OVERTAKE_SEGMENT            (    8U)  /* Overtake can be done starting from this segment */
#define SEGMENT_COUNT               (   16U)  /* Total number of the segments */

#define CAR_SPEED_STRAIGHT          ( 1.80f)   /* m/s */
#define CAR_SPEED_DECEL             ( 1.50f)   /* m/s */
#define CAR_SPEED_TURN              ( 1.20f)   /* m/s */
#define CAR_SPEED_ACCEL             ( 1.50f)   /* m/s */

#define CAR_WAIT_BEFORE_BRAKING     ( 2.00f)   /*   m */
#define CAR_WAIT_BEFORE_ACCEL       ( 0.30f)   /*   m */
#define CAR_WAIT_IN_LAST_LAP        ( 4.00f)   /*   m */

#define CAR_DIST_CTRL_P             ( 0.10f)
#define CAR_DIST_CTRL_D             ( 0.02f)

Car* Car::GetInstance()
{
    static Car instance;
    return &instance;
}

void Car::StateMachine()
{
    UpdateProperties();

    // Test
    // BasicDrive_StateMachine();
    //Follow_StateMachine();
    //BasicLabyrinth_StateMachine();

    // Race
#if USE_MINIMAL_STRATEGY == 1U
    Minimal_StateMachine();
#else
    RaceStateMachine();
#endif

    CheckDeadmanSwitch();

    Actuate();
}

void Car::SetSteeringMode(SteeringMode mode)
{
    wheels->SetMode(mode);
}

void Car::Reset_To_State(RaceState state)   // TODO test
{
    ChangeState(state);
}

void Car::Reset_To_FailedOvertake()         // TODO test and review
{
    ChangeState(RaceState::sp_Follow);
    ChangeRoadSegment(RoadSegment_SM::rs_Straight);
    segmentCounter = OVERTAKE_SEGMENT;
    carProp.targetSpeed = CAR_SPEED_STRAIGHT;
}

Car::Car()
{
    encoder       = Encoder::GetInstance();
    radio         = Starter::GetInstance();
    remote        = Remote::GetInstance();
    wheels        = Steering::GetInstance();
    motor         = Traction::GetInstance();
    lineSensor    = TrackDetector::GetInstance();
    distance      = Distance::GetInstance();
    clock         = Timepiece::GetInstance();
    trace         = StringQueue::GetInstance(Race);
    delayDistance = new WaitDistance();
    //delayTime     = new WaitTime();
    navigation    = Navigation::GetInstance();
    ui            = Ui::GetInstance();

    map = Map::GetInstance();
    nextTurn = TurnType::Right;

    recoverState             = la_Idle;
    carProp.state            = la_Idle;
    carProp.speed            = encoder->GetSpeed();
    carProp.dist_travelled   = encoder->GetDistance();
    carProp.track            = lineSensor->GetTrackType();
    carProp.position         = navigation->GetPosition();
    carProp.front_distance   = distance->GetDistance(DistanceSensor::ToF_Front);
    carProp.wheel_mode       = SingleLineFollow_Slow;
    carProp.targetSpeed      = 0U;
    carProp.lineFollow_Front = lineSensor->GetFrontLine();
    carProp.lineFollow_Rear  = 0U;
    carProp.sensorServoAngle = 0U;
    carProp.lineDetected     = false;

    speedRunStarted = false;
    roadSegment     = RoadSegment_SM::rs_Straight;
    segmentCounter  = 0U;
    lapFinished     = false;
    followLapCnt    = 0U;
    tryOvertake     = false;

    exitType        = TrackType::None;
    switchState     = LineSwitch_SM::PrepareForLaneChanging;
    reversingState  = Reversing_SM::PrepareForReversing;

    wheels->SetMode(carProp.wheel_mode);

    dist_ctrl = new Pid_Controller(CAR_DIST_CTRL_P, 0, CAR_DIST_CTRL_D);    // TODO
    dist_ctrl->SetSetpoint(CAR_FOLLOW_DISTANCE);

    prescaler = 0;

    // test
    recoverState             = la_Straight;
    carProp.state            = la_Straight;
    //speedRunStarted = true;
    //map->TurnOff();     // remove
    //lineSensor->SetMode(Speedrun); // Move to the end of the wait before speedrun
}

void Car::BasicLabyrinth_StateMachine()
{
    switch (carProp.state)
    {
        case la_Idle:
        {
            if ((USE_RADIO_STARTER == 0) || (radio->GetState() == Go))
            {
                lineSensor->SetMode(Maze);
                ChangeState(la_Straight);
                trace->Transmit("la_Straight", 11);
            }
            break;
        }
        case la_Straight:
        {
            carProp.lineFollow_Front = lineSensor->GetFrontLine(LineDirection::ld_Middle);
            carProp.lineFollow_Rear  = 0;
            carProp.targetSpeed      = MAZE_FORWARD_SPEED;

            if ((lineSensor->IsJunction(carProp.track)) || (lineSensor->IsFork(carProp.track))){        ChangeState(la_Junction);    trace->Transmit("la_Junction", 11);  }
            else if ((carProp.track == TrackType::Exit) || (carProp.track == TrackType::ExitReverse)){  ChangeState(la_Exit);        trace->Transmit("la_Exit", 7);       }
            else if (carProp.track == TrackType::DeadEnd){                                              ChangeState(la_Reverse);     trace->Transmit("la_Reverse", 10);   }
            else {}
            break;
        }
        case la_Junction:
        {
            carProp.lineFollow_Front = lineSensor->GetFrontLine(LineDirection::ld_Middle);
            carProp.lineFollow_Rear  = 0;
            carProp.targetSpeed      = MAZE_FORWARD_SLOW_SPEED;

            if (map->isDecisionMade() == true)
            {
                delayDistance->Wait_m(MAZE_TURN_WAIT_DIST);
                nextTurn = map->WhichWayToTurn();
                carProp.lineFollow_Front = lineSensor->GetFrontLine(SelectLineDirection(nextTurn));
                ChangeState(la_Turn);
                trace->Transmit("la_Turn", 7);
            }
            break;
        }
        case la_Turn:
        {
            carProp.wheel_mode = DualLineFollow_Slow;
            carProp.lineFollow_Front = lineSensor->GetFrontLine(SelectLineDirection(nextTurn));
            carProp.lineFollow_Rear = -carProp.lineFollow_Front;
            carProp.targetSpeed      = MAZE_FORWARD_SLOW_SPEED;

            if ((carProp.track == TrackType::Single) && (delayDistance->IsExpired() == true))
            {
                ChangeState(la_Straight);
                trace->Transmit("la_Straight", 11);
                carProp.wheel_mode = SingleLineFollow_Slow;
            }
            break;
        }
        case la_Reverse:
            Maneuver_Reverse();
            break;
        case la_Exit:
            if (true/*map->shouldExitMaze() == true*/){     Maneuver_ChangeLane();          }   // Q1
            else{                                   ChangeState(la_Straight);     trace->Transmit("la_Straight", 11); }
            break;
        case la_End:
            map->TurnOff();             // TODO don'T forget to activate for the race
            carProp.targetSpeed = 0U;
            break;
        default:
            break;  // Not a valid labyrinth state.
    }

    ui->SetCommand(carProp.state);
}

void Car::BaseRace_StateMachine()
{
    switch (carProp.state)
    {
        case sp_Wait:
        {
            ChangeState(sp_Follow);
            trace->Transmit("Speedrun is starting.", 21);
            speedRunStarted = true;
            map->TurnOff();
            lineSensor->SetMode(Speedrun);
            break;
        }
        case sp_Follow:
        {
            Follow_StateMachine();

            // Transitions
            if ((segmentCounter == OVERTAKE_SEGMENT) && (tryOvertake == true))  // Overtake is allowed and in the right segment.
            {
                ChangeState(sp_Overtake);
                trace->Transmit("sp_Overtake", 11);
            }
            if (lapFinished == true)                                            // Count the rounds.
            {
                followLapCnt++;
                lapFinished = false;
                trace->Transmit("Follow lap one completed", 24);
            }
            if (followLapCnt == 2)                                              // Must switch to Lap1 after 2 rounds.
            {
                ChangeState(sp_Lap1);
                followLapCnt  = 0;
                trace->Transmit("sp_Lap1", 7);
            }
            break;
        }
        case sp_Overtake:
        {
            Maneuver_Overtake();

            if ("success1"){        ChangeState(sp_Chase);          trace->Transmit("sp_Chase", 8);            }
            else if ("success2"){   ChangeState(sp_PrepareForLaps); trace->Transmit("sp_PrepareForLaps", 7);   }
            else{                   ChangeState(sp_Follow);         trace->Transmit("sp_Follow", 9);           }
            break;
        }
        case sp_Chase:
        {
            carProp.sensorServoAngle = wheels->GetFrontAngle()/2;
            // Transitions
            if ("safety car found")                 // Behind the safety car again.
            {
                ChangeState(sp_Follow);
                trace->Transmit("sp_Follow", 9);
            }
            if (lapFinished == true)                // Count the rounds.
            {
                followLapCnt++;
                lapFinished = false;
                trace->Transmit("Follow lap one completed", 24);
            }
            if (followLapCnt == 2)                  // Must switch to Lap1 after 2 rounds.
            {
                ChangeState(sp_Lap1);
                followLapCnt  = 0;
                trace->Transmit("sp_Lap1", 7);
            }
            break;
        }
        case sp_PrepareForLaps:
        {
            if (lapFinished == true){   ChangeState(sp_Lap1);    lapFinished = false;   trace->Transmit("sp_Lap1", 7); }
            break;
        }
        case sp_Lap1:
        {
            if (lapFinished == true){   ChangeState(sp_Lap2);    lapFinished = false;   trace->Transmit("sp_Lap2", 7); }
            break;
        }
        case sp_Lap2:
        {
            if (lapFinished == true){   ChangeState(sp_Lap3);    lapFinished = false;   trace->Transmit("sp_Lap3", 7); }
            break;
        }
        case sp_Lap3:
        {
            if (lapFinished == true)
            {
                ChangeState(sp_Stop);
                lapFinished = false;
                delayDistance->Wait_m(CAR_WAIT_IN_LAST_LAP);
                trace->Transmit("sp_Stop", 7);
            }
            break;
        }
        case sp_Stop:
        {
            if (delayDistance->IsExpired() == true){    carProp.targetSpeed = 0;    trace->Transmit("RACE IS OVER!", 13); }
            break;
        }
        default:
            break;
    }
}

void Car::RoadSegment_StateMachine()
{
    carProp.lineFollow_Front = lineSensor->GetFrontLine(LineDirection::ld_Middle);
    //carProp.lineFollow_Rear  = lineSensor->GetRearLine(LineDirection::ld_Middle);

    switch (roadSegment)
    {
        case RoadSegment_SM::rs_Straight:
        {
            carProp.wheel_mode = SteeringMode::SingleLine_Race_Straight;    // PD parameters
            carProp.targetSpeed = CAR_SPEED_STRAIGHT;

            if (lineSensor->GetTrackType() == TrackType::Braking)
            {
                delayDistance->Wait_m(CAR_WAIT_BEFORE_BRAKING);
                ChangeRoadSegment(RoadSegment_SM::rs_Decelerate);
                trace->Transmit("...rs_Decelerate", 16);
            }
            break;
        }
        case RoadSegment_SM::rs_Decelerate:
        {
            carProp.wheel_mode= SteeringMode::SingleLine_Race_Decel;
            carProp.targetSpeed = CAR_SPEED_DECEL;

            if (delayDistance->IsExpired() == true)
            {
                ChangeRoadSegment(RoadSegment_SM::rs_Turn);
                trace->Transmit("...rs_Turn", 10);
            }
            break;
        }
        case RoadSegment_SM::rs_Turn:
        {
            carProp.wheel_mode = SteeringMode::SingleLine_Race_Turn;        // TODO check double axles turn
            carProp.targetSpeed = CAR_SPEED_TURN;

            if (lineSensor->GetTrackType() == TrackType::Acceleration)
            {
                delayDistance->Wait_m(CAR_WAIT_BEFORE_ACCEL);
                ChangeRoadSegment(RoadSegment_SM::rs_Accelerate);
                trace->Transmit("...rs_Accelerate", 16);
            }
            break;
        }
        case RoadSegment_SM::rs_Accelerate:
        {
            carProp.wheel_mode = SteeringMode::SingleLine_Race_Accel;
            carProp.targetSpeed = CAR_SPEED_ACCEL;

            if (delayDistance->IsExpired() == true)
            {
                ChangeRoadSegment(RoadSegment_SM::rs_Straight);
                trace->Transmit("...rs_Straight", 14);
            }
            break;
        }
        default:
            break;
    }
}

void Car::Follow_StateMachine()
{
    if (carProp.front_distance > CAR_FOLLOW_MAX_APPROX)
    {
        carProp.targetSpeed = CAR_FOLLOW_MAX_SPEED;
    }
    else
    {
        if (carProp.front_distance > CAR_FOLLOW_DISTANCE)
        {
            carProp.targetSpeed = CAR_FOLLOW_REDUCED_SPEED;
        }
        else if (carProp.front_distance < CAR_FOLLOW_MIN_APPROX)
        {
            if (carProp.speed > 0.05f)
            {
                carProp.targetSpeed = CAR_FOLLOW_REV_SPEED;
            }
            else
            {
                carProp.targetSpeed = 0.0f;
            }
        }
        else
        {
            carProp.targetSpeed = 0;
        }
    }

    // Sensor Direction.
    carProp.lineFollow_Front = lineSensor->GetFrontLine(LineDirection::ld_Middle);
    carProp.sensorServoAngle =  wheels->GetFrontAngle()/2;
}

void Car::Minimal_StateMachine()
{
    if (speedRunStarted == true)
    {
        motor->SetMode(tmode_Controller);
        RoadSegment_StateMachine();
    }
    else
    {
        motor->SetMode(tmode_Manual);
        BasicLabyrinth_StateMachine();
    }

    BaseRace_StateMachine();
}

void Car::Race_StateMachine()
{
    // Advanced labyrinth
    // Advanced speed run
}

void Car::Maneuver_Reverse()
{
    switch (reversingState) // combinations: fork -> single; junction -> single || fork
    {
        case Reversing_SM::PrepareForReversing:
        {
            carProp.wheel_mode = SteeringMode::DualLineFollow_Slow;
            carProp.lineFollow_Front = lineSensor->GetFrontLine();
            carProp.lineFollow_Rear  = lineSensor->GetRearLine();

            //carProp.targetSpeed = 0;
            reversingState = Reversing_SM::Reversing;
            trace->Transmit("_____REV: Reversing started.", 28);
            break;
        }
        case Reversing_SM::Reversing:
        {
            carProp.wheel_mode = SteeringMode::DualLineFollow_Slow;
            carProp.lineFollow_Front = 0;
            carProp.lineFollow_Rear  = lineSensor->GetRearLine();
            carProp.targetSpeed      = MAZE_REVERSE_SPEED;

            if ((lineSensor->IsFork(carProp.track)) || (lineSensor->IsJunction(carProp.track)))
            {
                reversingState = Reversing_SM::JunctionFound;
                trace->Transmit("_____REV: Junction Found.", 25);
            }
            break;
        }
        case Reversing_SM::JunctionFound:
        {
            carProp.wheel_mode = SteeringMode::SingleLineFollow_Slow;

            // Dummy decision not to turn to the dead end again. Assumption: turn left means the left most line
            if (nextTurn == TurnType::Left)
            {
                nextTurn = TurnType::Right;
                carProp.lineFollow_Front = lineSensor->GetFrontLine(LineDirection::ld_Right);
                //carProp.lineFollow_Rear  = lineSensor->GetRearLine(LineDirection::ld_Middle);
            }
            else
            {
                nextTurn = TurnType::Left;
                carProp.lineFollow_Front = lineSensor->GetFrontLine(LineDirection::ld_Left);
                //carProp.lineFollow_Rear  = lineSensor->GetRearLine(LineDirection::ld_Middle);
            }

            carProp.targetSpeed      = MAZE_FORWARD_SPEED;

            // Reset state machine for next time
            reversingState = Reversing_SM::PrepareForReversing;
            trace->Transmit("_____REV: Reversing maneuver is finished", 40);
            ChangeState(la_Turn);
            trace->Transmit("la_Turn", 7);
            delayDistance->Wait_m(MAZE_TURN_WAIT_DIST);
            break;
        }
        default:
            break;
    }
}

void Car::Maneuver_ChangeLane()     // TODO end feature
{
    switch (switchState)
    {
        case LineSwitch_SM::PrepareForLaneChanging:
        {
            exitType = carProp.track;
            carProp.wheel_mode = SteeringMode::Free;

            if (exitType == TrackType::Exit)
            {
                delayDistance->Wait_m(-0.6f);
                switchState = LineSwitch_SM::right_Prep;
                trace->Transmit("Exit: Started right", 19);
            }
            else if (exitType == TrackType::ExitReverse)
            {
                delayDistance->Wait_m(1.4f);
                switchState = LineSwitch_SM::rev_Y_Prep;
                trace->Transmit("Exit started Y", 14);
            }
            else {} // Invalid state
            break;
        }
        case LineSwitch_SM::right_Prep:
        {
            carProp.wheel_mode = SteeringMode::DualLineFollow_Slow;
            carProp.lineFollow_Front = 0;
            carProp.lineFollow_Rear  = lineSensor->GetRearLine();
            carProp.targetSpeed      = -MAZE_FORWARD_SPEED;

            if (delayDistance->IsExpired() == true)
            {
                delayDistance->Wait_m(MAZE_EXIT_WAIT_DIST);
                carProp.wheel_mode = SteeringMode::Free;
                switchState = LineSwitch_SM::right_LeaveLine;
                trace->Transmit("Exit: R back", 13);
            }
            break;
            break;
        }
        case LineSwitch_SM::right_LeaveLine:
        {
            // Special case. Mode is changed manually.
            carProp.wheel_mode = SteeringMode::Free;
            wheels->SetAngleManual(MAZE_EXIT_WHEEL_ANGLE, MAZE_EXIT_WHEEL_ANGLE+0.2f);
            carProp.targetSpeed = MAZE_EXIT_SPEED;

            if (delayDistance->IsExpired() == true && !carProp.lineDetected)
            {
                delayDistance->Wait_m(MAZE_EXIT_DIST_LIMIT);
                switchState = LineSwitch_SM::right_SearchLineOnRight;
                trace->Transmit("Exit: Left the maze", 19);
            }
            break;
        }
        case LineSwitch_SM::right_SearchLineOnRight:
        {
            carProp.wheel_mode = SteeringMode::Free;
            wheels->SetAngleManual(MAZE_EXIT_WHEEL_ANGLE, MAZE_EXIT_WHEEL_ANGLE);
            carProp.targetSpeed = MAZE_EXIT_SPEED;

            // Wait until 1 line
            if (carProp.lineDetected)
            {
                carProp.wheel_mode = SteeringMode::SingleLineFollow_Slow;
                switchState = LineSwitch_SM::LineFound;
                trace->Transmit("Exit: Line found", 16);
            }

            if (delayDistance->IsExpired() == true)
            {
                switchState = LineSwitch_SM::NoLineFound;
            }
            break;
        }
        case LineSwitch_SM::rev_Y_Prep:
        {
            carProp.wheel_mode = SteeringMode::SingleLineFollow_Slow;
            carProp.lineFollow_Front = lineSensor->GetFrontLine(LineDirection::ld_Middle);
            carProp.lineFollow_Rear  = 0;
            carProp.targetSpeed      = MAZE_FORWARD_SPEED;

            if (delayDistance->IsExpired() == true)
            {
                delayDistance->Wait_m(-MAZE_EXIT_REV_WAIT_DIST_1);
                carProp.wheel_mode = SteeringMode::Free;
                switchState = LineSwitch_SM::rev_Y_Reverse1;
                trace->Transmit("Exit: Y turn back", 17);
            }
            break;
        }
        case LineSwitch_SM::rev_Y_Reverse1:
        {
            carProp.wheel_mode = SteeringMode::Free;
            wheels->SetAngleManual(MAZE_EXIT_WHEEL_ANGLE, -MAZE_EXIT_WHEEL_ANGLE);
            carProp.targetSpeed = -MAZE_EXIT_SPEED;

            if (delayDistance->IsExpired() == true)
            {
                delayDistance->Wait_m(MAZE_EXIT_REV_WAIT_DIST_2);
                carProp.wheel_mode = SteeringMode::Free;
                switchState = LineSwitch_SM::rev_Y_TurnLeft;
                trace->Transmit("Exit: Y turn left", 17);
            }
            break;
        }
        case LineSwitch_SM::rev_Y_TurnLeft:
        {
            carProp.wheel_mode = SteeringMode::Free;
            wheels->SetAngleManual(-MAZE_EXIT_WHEEL_ANGLE, MAZE_EXIT_WHEEL_ANGLE);
            carProp.targetSpeed = MAZE_EXIT_SPEED;

            if (delayDistance->IsExpired() == true)
            {
                delayDistance->Wait_m(-MAZE_EXIT_REV_WAIT_DIST_3);
                carProp.wheel_mode = SteeringMode::Free;
                switchState = LineSwitch_SM::rev_Y_LeaveLine;
                trace->Transmit("Exit: Y rev 2", 13);
            }
            break;
        }
        case LineSwitch_SM::rev_Y_Reverse2:
        {
            carProp.wheel_mode = SteeringMode::Free;
            wheels->SetAngleManual(MAZE_EXIT_WHEEL_ANGLE, -MAZE_EXIT_WHEEL_ANGLE);
            carProp.targetSpeed = -MAZE_EXIT_SPEED;

            if (delayDistance->IsExpired() == true)
            {
                delayDistance->Wait_m(MAZE_EXIT_REV_WAIT_DIST_4);
                carProp.wheel_mode = SteeringMode::Free;
                switchState = LineSwitch_SM::rev_Y_LeaveLine;
                trace->Transmit("Exit: Y search line", 19);
            }
            break;
        }
        case LineSwitch_SM::rev_Y_LeaveLine:
        {
            carProp.wheel_mode = SteeringMode::Free;
            wheels->SetAngleManual(MAZE_EXIT_WHEEL_ANGLE/2, 0);
            carProp.targetSpeed = MAZE_EXIT_SPEED;

            // Wait until 1 line
            if (carProp.lineDetected == true)
            {
                carProp.wheel_mode = SteeringMode::SingleLineFollow_Slow;
                switchState = LineSwitch_SM::LineFound;
                trace->Transmit("Exit: Line found", 16);
            }

            if (delayDistance->IsExpired() == true)
            {
                switchState = LineSwitch_SM::NoLineFound;
            }
            break;
        }
        case LineSwitch_SM::LineFound:     ChangeState(sp_Wait);  switchState = LineSwitch_SM::PrepareForLaneChanging;  trace->Transmit("Labyrinth is over.", 18);   break;
        case LineSwitch_SM::NoLineFound:   ChangeState(la_End);   switchState = LineSwitch_SM::PrepareForLaneChanging;  trace->Transmit("Lost", 4);                  break;
        default:
            break;
    }
}

void Car::Maneuver_Overtake()       // TODO end feature.
{
    // Steer left.
    // Speed up to overtake.
    // Steer right.
}

void Car::CheckDeadmanSwitch()
{
    if (remote->GetValue(RemoteChannel::ThrottleCh) < 0.1f)
    {
        // No throttle
        motor->SetMode(tmode_Manual);
        carProp.targetSpeed = 0.0f;
        carProp.state       = RaceState::la_End;
    }
    else
    {
        // Throttle
        if (carProp.state != recoverState)
        {
            Trace::Print(trace, "Dead-man switch recovered state");
        }

        carProp.state = recoverState;
    }
}

void Car::UpdateProperties()
{
    carProp.speed           = encoder->GetSpeed();
    carProp.dist_travelled  = encoder->GetDistance();
    carProp.track           = lineSensor->GetTrackType();
    carProp.position        = navigation->GetPosition();
    carProp.front_distance  = distance->GetDistance(DistanceSensor::ToF_Front);
    carProp.lineDetected    = lineSensor->IsFrontLineDetected();
}

void Car::Actuate()
{
    if (speedRunStarted == true){
        motor->SetMode(tmode_Controller);
        motor->SetSpeed(carProp.targetSpeed);
    }
    else{
        motor->SetDutyCycle(carProp.targetSpeed);
    }

    wheels->SetMode(carProp.wheel_mode);
    wheels->SetLine(carProp.lineFollow_Front, carProp.lineFollow_Rear);

    if (speedRunStarted == true){   distance->SetFrontServo(carProp.sensorServoAngle); }
    else{                           distance->SetFrontServo(0);   }
}

void Car::ChangeState(RaceState const State)
{
    carProp.state = State;
    recoverState  = State;
}

void Car::ChangeRoadSegment(RoadSegment_SM const Segment)
{
    roadSegment = Segment;
    ui->SetCommand(segmentCounter);

    if (segmentCounter < SEGMENT_COUNT - 1)
    {
        lapFinished = false;
        segmentCounter++;

        switch (segmentCounter)
        {
            case 1:     trace->Transmit("SP segment: 1",  13);   break;
            case 2:     trace->Transmit("SP segment: 2",  13);   break;
            case 3:     trace->Transmit("SP segment: 3",  13);   break;
            case 4:     trace->Transmit("SP segment: 4",  13);   break;
            case 5:     trace->Transmit("SP segment: 5",  13);   break;
            case 6:     trace->Transmit("SP segment: 6",  13);   break;
            case 7:     trace->Transmit("SP segment: 7",  13);   break;
            case 8:     trace->Transmit("SP segment: 8 overtake possible",  13);   break;
            case 9:     trace->Transmit("SP segment: 9",  13);   break;
            case 10:    trace->Transmit("SP segment: 10", 14);   break;
            case 11:    trace->Transmit("SP segment: 11", 14);   break;
            case 12:    trace->Transmit("SP segment: 12", 14);   break;
            case 13:    trace->Transmit("SP segment: 13", 14);   break;
            case 14:    trace->Transmit("SP segment: 14", 14);   break;
            case 15:    trace->Transmit("SP segment: 15", 14);   break;
            default:
                break;
        }
    }
    else
    {
        lapFinished = true;
        trace->Transmit("SP segment: 0",  13);
        segmentCounter = 0;
    }
}

LineDirection Car::SelectLineDirection(TurnType const turn)
{
    LineDirection dir = LineDirection::ld_NoLine;

    switch (turn)
    {
        case TurnType::Left:    dir = LineDirection::ld_Left;      break;
        case TurnType::NoTurn:  dir = LineDirection::ld_Middle;    break;
        case TurnType::Right:   dir = LineDirection::ld_Right;     break;
        default:
            break;
    }

    return dir;
}
