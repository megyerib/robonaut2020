#include "Car.h"

#define OVERTAKE_SEGMENT       (8U)    /* Overtake can be done starting from this segment */

#define MAZE_FORWARD_SPEED          ( 0.14f)  /* % */
#define MAZE_REVERSE_SPEED          (-0.11f)  /* % */

#define CAR_FOLLOW_DISTANCE         (0.6f)    /* m */
#define CAR_FOLLOW_MIN_APPROX       (0.4f)    /* m */
#define CAR_FOLLOW_MAX_SPEED        (0.15f)   /* % */
#define CAR_FOLLOW_REV_SPEED        (-0.1f)   /* % */

#define CAR_SPEED_STRAIGHT          (0.22f)   /* % */
#define CAR_SPEED_DECEL             (0.19)    /* % */
#define CAR_SPEED_TURN              (0.16f)   /* % */
#define CAR_SPEED_ACCEL             (0.19f)   /* % */

#define CAR_WAIT_BEFORE_BRAKING     (1.0f)    /* m */
#define CAR_WAIT_BEFORE_ACCEL       (0.3f)    /* m */

#define CAR_DIST_CTRL_P             (0.1f)
#define CAR_DIST_CTRL_D             (0.02f)

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
    //BasicDrive_StateMachine();
    Follow_StateMachine();
    //BasicLabyrinth_StateMachine();

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
   // trace         = Trace::GetInstance();
    delayDistance = new WaitDistance();
    //delayTime     = new WaitTime();
    navigation    = Navigation::GetInstance();

    map = Map::GetInstance();
    nextTurn = TurnType::NotSet;

    recoverState             = sp_Wait;
    carProp.state            = sp_Wait;
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

    switchState     = LineSwitch_SM::LeaveLine;

    wheels->SetMode(carProp.wheel_mode);

    dist_ctrl = new Pid_Controller(CAR_DIST_CTRL_P, 0, CAR_DIST_CTRL_D);    // TODO
    dist_ctrl->SetSetpoint(CAR_FOLLOW_DISTANCE);

    prescaler = 0;
}

void Car::CheckDeadmanSwitch()
{
    if (remote->GetValue(RemoteChannel::ThrottleCh) < 0.1f)
    {
        carProp.state = sp_Stop;
        carProp.speed = 0.0f;
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
            motor->SetDutyCycle(CAR_SPEED_TURN);

            // Direction
            //wheels->SetLine(lineSensor->GetFrontLine(), 0);

            // Transition
            if (lineSensor->GetTrackType() == TrackType::Acceleration)
            {
                recoverState  = sp_Accelerate;
                carProp.state = sp_Accelerate;
                motor->SetDutyCycle(CAR_SPEED_STRAIGHT - 0.02f);
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
            motor->SetDutyCycle(0.0f);
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
    wheels->SetLine(lineSensor->GetFrontLine(), 0);
    distance->SetFrontServo(wheels->GetFrontAngle()/2);
}

void Car::BasicLabyrinth_StateMachine()
{
    switch (carProp.state)
    {
        case la_Idle:
        {
            if ((USE_RADIO_STARTER == 0) || (radio->GetState() == Go)){ carProp.state = la_Straight; }
            break;
        }
        case la_Straight:
        {
            carProp.lineFollow_Front = lineSensor->GetFrontLine(LineDirection::ld_Middle);
            //carProp.lineFollow_Rear  = lineSensor->GetRearLine(LineDirection::ld_Middle);
            carProp.targetSpeed      = MAZE_FORWARD_SPEED;

            if ((lineSensor->IsJunction(carProp.track)) || (lineSensor->IsFork(carProp.track))){        carProp.state = la_Junction;    }
            else if ((carProp.track == TrackType::Exit) || (carProp.track == TrackType::ExitReverse)){  carProp.state = la_Exit;        }
            else if (carProp.track == TrackType::DeadEnd){                                              carProp.state = la_Reverse;     }
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
                nextTurn = map->WhichWayToTurn();
                carProp.state = la_Turn;
            }
            break;
        }
        case la_Turn:
        {
            carProp.lineFollow_Front = lineSensor->GetFrontLine(SelectLineDirection(nextTurn));
            //carProp.lineFollow_Rear  = lineSensor->GetRearLine(SelectLineDirection(nextTurn));
            carProp.targetSpeed      = MAZE_FORWARD_SPEED - 0.015f;

            if (carProp.track == TrackType::Single){  carProp.state = la_Straight;    }
            break;
        }
        case la_Reverse:
            // TODO think it over
            Maneuver_Reverse();
            if (lineSensor->IsJunction(carProp.track)){    carProp.state = la_Junction;    }
            break;
        case la_Exit:
            Maneuver_ChangeLane();
        case la_End:
            carProp.state = sp_Wait;
            break;
        default:
            break;  // Not a valid labyrinth state.
    }
}

void Car::Race_StateMachine()
{
    switch (carProp.state)
    {
        case la_Idle:
            if ((USE_RADIO_STARTER == 1) && (radio->GetState() == Go)){ carProp.state = la_Straight; }
            break;
        case la_Straight:
            if ("Junction"){        carProp.state = la_Junction;    }
            else if ("Exit"){       carProp.state = la_Exit;        }
            else if ("dead end"){   carProp.state = la_Reverse;     }
            else {}
            break;
        case la_Junction:
            if ("decision made"){   carProp.state = la_Turn;    }
            break;
        case la_Turn:
            if ("left the junction"){  carProp.state = la_Straight;    }
            break;
        case la_Reverse:
            // maneuver
            if ("reached the junction"){    carProp.state = la_Junction;    }
            break;
        case la_Exit:
            // normal or opposite exit maneuver
            if ("successful maneuver"){     carProp.state = la_End; }
        case la_End:
            carProp.state = sp_Wait;
            break;
        case sp_Wait:
            carProp.state = sp_Follow;
            break;
        case sp_Follow:
            // Follow state machine
            if ("in proper segment"){   carProp.state = sp_Overtake;    }
            break;
        case sp_Overtake:
            // maneuver
            if ("maneuver success"){    carProp.state = sp_Straight;    }
            else if ("fail"){           carProp.state = sp_Follow;      }
            else {}
            break;
        // TODO
        default:
            break;
    }
}

void Car::Minimal_StateMachine()
{

}

void Car::Maneuver_Reverse()
{

}

void Car::Maneuver_ChangeLane()     // TODO end feature
{
    switch (switchState)
    {
        case LineSwitch_SM::LeaveLine:
        {
            // Start distance measure
            // Adjust speed
            // Line
            if (carProp.track == TrackType::Exit)
            {
                // Turn right
                switchState = LineSwitch_SM::SearchLine;
            }
            if (carProp.track == TrackType::ExitReverse)
            {
                // Turn left
                switchState = LineSwitch_SM::SearchLine;
            }
            break;
        }
        case LineSwitch_SM::SearchLine:
        {
            // Go straight
            // Line
            if (carProp.track == TrackType::Single)
            {
                switchState = LineSwitch_SM::LineFound;
            }

            if ("distance finished")
            {
                switchState = LineSwitch_SM::NoLineFound;
            }
            break;
        }
        case LineSwitch_SM::LineFound:     carProp.state = RaceState::la_End; /* sp_Follow; */   break;
        case LineSwitch_SM::NoLineFound:   carProp.state = RaceState::la_End;      break;
        default:
            break;
    }
}

void Car::Maneuver_Overtake()
{

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
