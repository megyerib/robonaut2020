#include "Car.h"

#define MAZE_FORWARD_SPEED          ( 0.17f)  /*   % */ //!< Speed used in the labyrinth when the robot is over a Single line.
#define MAZE_REVERSE_SPEED          (-0.15f)  /*   % */ //!< Speed used for reverse maneuver.
#define MAZE_REVERSE_P              ( 20.0f)
#define MAZE_REVERSE_D              (200.0f)
#define MAZE_EXIT_WAIT_DIST         ( 0.20f)  /*   m */ //!< Distance the robot will make to leave the labyrinth, before searching for the speedrun line.
#define MAZE_EXIT_WHEEL_ANGLE       ( 0.52f)  /* rad */ //!< The angles of the servos, while leaving the labyrinth.
#define MAZE_EXIT_DIST_LIMIT        ( 0.70f)  /*   m */ //!< The distance how many meters can the robot drive while searching for the speedrun line.
#define MAZE_EXIT_SPEED             ( 0.12f)  /*   % */

#define CAR_FOLLOW_DISTANCE         ( 0.60f)  /* m */
#define CAR_FOLLOW_MIN_APPROX       ( 0.40f)  /* m */
#define CAR_FOLLOW_MAX_SPEED        ( 0.15f)  /* % */
#define CAR_FOLLOW_REV_SPEED        (-0.10f)  /* % */

#define OVERTAKE_SEGMENT            (    8U)  /* Overtake can be done starting from this segment */

#define CAR_SPEED_STRAIGHT          ( 0.22f)   /* % */
#define CAR_SPEED_DECEL             ( 0.19f)   /* % */
#define CAR_SPEED_TURN              ( 0.16f)   /* % */
#define CAR_SPEED_ACCEL             ( 0.19f)   /* % */

#define CAR_WAIT_BEFORE_BRAKING     ( 1.00f)   /* m */
#define CAR_WAIT_BEFORE_ACCEL       ( 0.30f)   /* m */

#define CAR_DIST_CTRL_P             ( 0.10f)
#define CAR_DIST_CTRL_D             ( 0.02f)

Car* Car::GetInstance()
{
    static Car instance;
    return &instance;
}

void Car::StateMachine()
{
    CheckDeadmanSwitch();

    UpdateProperties();

    // Test
    // BasicDrive_StateMachine();
    Follow_StateMachine();
    // BasicLabyrinth_StateMachine();

    // Race
#if USE_MINIMAL_STRATEGY == 1U
    //Minimal_StateMachine();
#else
    RaceStateMachine();
#endif

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

void Car::Reset_To_FailedOvertake()         // TODO test
{
    ChangeState(RaceState::la_Straight);
    actLap         = Lap::Follow_Two;
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

    map = Map::GetInstance();
    nextTurn = TurnType::NotSet;

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

    actLap          = Lap::InvalidLap;
    segmentCounter  = 0U;

    exitType        = TrackType::None;
    switchState     = LineSwitch_SM::LeaveLine;
    reversingState  = Reversing_SM::PrepareForReversing;

    wheels->SetMode(carProp.wheel_mode);

    dist_ctrl = new Pid_Controller(CAR_DIST_CTRL_P, 0, CAR_DIST_CTRL_D);    // TODO
    dist_ctrl->SetSetpoint(CAR_FOLLOW_DISTANCE);

    prescaler = 0;
}

void Car::ChangeState(RaceState const State)
{
    carProp.state = State;
    recoverState  = State;
}

void Car::CheckDeadmanSwitch()
{
    if (remote->GetValue(RemoteChannel::ThrottleCh) < 0.1f)
    {
        carProp.state       = RaceState::la_End;
        carProp.targetSpeed = 0.0f;
    }
    else
    {
        carProp.state = recoverState;
    }
}

void Car::BasicDrive_StateMachine()
{
    switch (carProp.state)
    {
        case sp_Wait:
        {
            //if (radio->GetState() == StarterState::Go)
            {
                carProp.state = sp_Follow;
                wheels->SetMode(SingleLineFollow_Slow);
            }
            break;
        }
        case sp_Straight:
        {
            carProp.targetSpeed = CAR_SPEED_STRAIGHT;

            // Direction
            //wheels->SetLine(lineSensor->GetFrontLine(), 0);

            // Transition
            if (carProp.track == TrackType::Braking)
            {
                recoverState  = sp_Decelerate;
                carProp.state = sp_Decelerate;
                carProp.targetSpeed = (CAR_SPEED_STRAIGHT - 0.02f);
                wheels->SetMode(SingleLine_Race_Decel);
                delayDistance->Wait_m(CAR_WAIT_BEFORE_BRAKING);
                //delayTime->Wait_ms(1000);
            }
            break;
        }
        case sp_Decelerate:
        {
            // Wait distance
            if (delayDistance->IsExpired() == true)
            //if (delayTime->IsExpired() == true)
            {
                // Adjust controllers
                wheels->SetMode(SingleLine_Race_Turn);

                // Transition
                recoverState  = sp_Turn;
                carProp.state = sp_Turn;
            }
            break;
        }
        case sp_Turn:
        {
            // Speed
            ///motor->SetDutyCycle(CAR_SPEED_TURN);

            // Direction
            //wheels->SetLine(lineSensor->GetFrontLine(), 0);

            // Transition
            if (lineSensor->GetTrackType() == TrackType::Acceleration)
            {
                recoverState  = sp_Accelerate;
                carProp.state = sp_Accelerate;
                ///motor->SetDutyCycle(CAR_SPEED_STRAIGHT - 0.02f);
                wheels->SetMode(SingleLine_Race_Accel);
                delayDistance->Wait_m(CAR_WAIT_BEFORE_ACCEL);
                //delayTime->Wait_ms(500);
            }
            break;
        }
        case sp_Accelerate:
        {
            // Wait distance
            if (delayDistance->IsExpired() == true)
            //if (delayTime->IsExpired() == true)
            {
                // Adjust controllers
                wheels->SetMode(DualLine_Race_Straight);

                // Transition
                recoverState  = sp_Straight;
                carProp.state = sp_Straight;
            }
            break;
        }
        case sp_Follow:
        {
            Follow_StateMachine();
            break;
        }
        case sp_Stop:
        {
           /// motor->SetDutyCycle(0.0f);
            //wheels->SetLine(lineSensor->GetFrontLine(), 0);
            break;
        }
        default:
        {
            break;
        }
    }
}

void Car::Follow_StateMachine()
{
    if (carProp.front_distance > CAR_FOLLOW_DISTANCE)
    {
        carProp.targetSpeed = CAR_FOLLOW_MAX_SPEED;
    }
    else if (carProp.front_distance < CAR_FOLLOW_MIN_APPROX)
    {
        if (carProp.speed > 0)
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
        carProp.targetSpeed = (carProp.front_distance - CAR_FOLLOW_MIN_APPROX) / 0.2 * CAR_FOLLOW_MAX_SPEED;
    }

    // Sensor Direction.
    carProp.lineFollow_Front = lineSensor->GetFrontLine();
    distance->SetFrontServo(wheels->GetFrontAngle()/2); // TODO Add to carProp
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
            //carProp.lineFollow_Rear  = lineSensor->GetRearLine(LineDirection::ld_Middle);
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
            //carProp.lineFollow_Rear  = lineSensor->GetRearLine(LineDirection::ld_Middle);
            carProp.targetSpeed      = MAZE_FORWARD_SPEED - 0.015f;

            if (map->isDecisionMade() == true)
            {
                delayDistance->Wait_m(0.60f);
                nextTurn = map->WhichWayToTurn();
                ChangeState(la_Turn);
                trace->Transmit("la_Turn", 7);
            }
            break;
        }
        case la_Turn:
        {
            carProp.lineFollow_Front = lineSensor->GetFrontLine(SelectLineDirection(nextTurn));
            //carProp.lineFollow_Rear  = lineSensor->GetRearLine(SelectLineDirection(nextTurn));
            carProp.targetSpeed      = MAZE_FORWARD_SPEED - 0.015f;

            if ((carProp.track == TrackType::Single) && (delayDistance->IsExpired() == true))
            {
                ChangeState(la_Straight);
                trace->Transmit("la_Straight", 11);
            }
            break;
        }
        case la_Reverse:
            Maneuver_Reverse();
            break;
        case la_Exit:
            if (map->shouldExitMaze() == true){     /*Maneuver_ChangeLane();*/              }   // Q1
            else{                                   ChangeState(la_Straight);     trace->Transmit("la_Straight", 11); }
            break;
        case la_End:
            //ChangeState(sp_Wait);
            // trace->Transmit("sp_Wait", 7);
            carProp.targetSpeed = 0U;   // TODO remove just for test
            break;
        default:
            break;  // Not a valid labyrinth state.
    }
}

void Car::Race_StateMachine()
{

}

void Car::Minimal_StateMachine()
{
    BasicLabyrinth_StateMachine();
    BasicDrive_StateMachine();
}

void Car::Maneuver_Reverse()
{
    switch (reversingState) // combinations: fork -> single; junction -> single || fork
    {
        case Reversing_SM::PrepareForReversing:
        {
            wheels->SetMode(SteeringMode::DualLineFollow_Slow);
            carProp.lineFollow_Front = lineSensor->GetFrontLine();
            carProp.lineFollow_Rear  = lineSensor->GetRearLine();

            //carProp.targetSpeed = 0;
            reversingState = Reversing_SM::Reversing;
            trace->Transmit("_____REV: Reversing started.", 28);
            break;
        }
        case Reversing_SM::Reversing:
        {
            wheels->SetMode(SteeringMode::DualLineFollow_Slow);
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
            wheels->SetMode(SteeringMode::SingleLineFollow_Slow);

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
            delayDistance->Wait_m(0.60f);
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
            wheels->SetMode(SteeringMode::Free);

            if (exitType == TrackType::Exit)
            {
                delayDistance->Wait_m(MAZE_EXIT_WAIT_DIST);
                switchState = LineSwitch_SM::LeaveLine;
            }
            else if (exitType == TrackType::ExitReverse)
            {
                delayDistance->Wait_m(MAZE_EXIT_WAIT_DIST);
                switchState = LineSwitch_SM::Y_part1;
            }
            else {} // Invalid state
            break;
        }
        case LineSwitch_SM::Y_part1:
            //TODO
            switchState = LineSwitch_SM::Y_part2;
            break;
        case LineSwitch_SM::Y_part2:
            //TODO
            switchState = LineSwitch_SM::LineFound;
            break;
        case LineSwitch_SM::LeaveLine:
        {
            wheels->SetMode(SteeringMode::Free);
            wheels->SetAngleManual(MAZE_EXIT_WHEEL_ANGLE, MAZE_EXIT_WHEEL_ANGLE);
            carProp.targetSpeed = MAZE_EXIT_SPEED;

            if (delayDistance->IsExpired() == true)
            {
                delayDistance->Wait_m(MAZE_EXIT_DIST_LIMIT);
                switchState = LineSwitch_SM::SearchLineOnRight;
            }
            break;
        }
        case LineSwitch_SM::SearchLineOnRight:
        {
            wheels->SetMode(SteeringMode::Free);
            wheels->SetAngleManual(MAZE_EXIT_WHEEL_ANGLE, MAZE_EXIT_WHEEL_ANGLE);
            carProp.targetSpeed = MAZE_EXIT_SPEED;

            if (carProp.track == TrackType::Single)
            {
                wheels->SetMode(SteeringMode::SingleLineFollow_Slow);
                switchState = LineSwitch_SM::LineFound;
            }

            if (delayDistance->IsExpired() == true)
            {
                switchState = LineSwitch_SM::NoLineFound;
            }
            break;
        }
        case LineSwitch_SM::LineFound:     ChangeState(la_End); /* sp_Follow; */  switchState = LineSwitch_SM::PrepareForLaneChanging;   break;
        case LineSwitch_SM::NoLineFound:   ChangeState(la_End);     switchState = LineSwitch_SM::PrepareForLaneChanging;      break;
        default:
            break;
    }
}

void Car::Maneuver_Overtake()   // TODO end feature.
{
    // TODO
    // Steer left.
    // Speed up to overtake.
    // Steer right.
}

void Car::UpdateProperties()
{
    carProp.speed           = encoder->GetSpeed();
    carProp.dist_travelled  = encoder->GetDistance();
    carProp.track           = lineSensor->GetTrackType();
    carProp.position        = navigation->GetPosition();
    carProp.front_distance  = distance->GetDistance(DistanceSensor::ToF_Front);
}

void Car::Actuate()
{
    motor->SetDutyCycle(carProp.targetSpeed);
    wheels->SetLine(carProp.lineFollow_Front, carProp.lineFollow_Rear);
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
