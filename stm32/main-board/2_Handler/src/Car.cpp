#include "Car.h"

#define CAR_SPEED_STRAIGHT          (0.22f)   /* % */
#define CAR_SPEED_DECEL             (0.19)    /* % */
#define CAR_SPEED_TURN              (0.16f)   /* % */
#define CAR_SPEED_ACCEL             (0.19f)   /* % */

#define CAR_WAIT_BEFORE_BRAKING     (1.0f)    /* m */
#define CAR_WAIT_BEFORE_ACCEL       (0.3f)   /* m */

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

    switch (state)
    {
        case QualiState::Wait:
        {
            //if (radio->GetState() == StarterState::Go)
            {
                state = QualiState::Follow;
                wheels->SetMode(SingleLineFollow_Slow);
            }
            break;
        }
        case Straight:
        {
            // Speed
            motor->SetDutyCycle(CAR_SPEED_STRAIGHT);

            // Direction
            //wheels->SetLine(lineSensor->GetFrontLine(), 0);

            // Transition
            if (lineSensor->GetTrackType() == TrackType::Braking)
            {
                recover = state = QualiState::Decelerate;
                motor->SetDutyCycle(CAR_SPEED_STRAIGHT - 0.02f);
                wheels->SetMode(SingleLine_Race_Decel);
                delayDistance->Wait_m(CAR_WAIT_BEFORE_BRAKING);
                //delayTime->Wait_ms(1000);
            }
            break;
        }
        case Decelerate:
        {
            // Wait distance
            if (delayDistance->IsExpired() == true)
            //if (delayTime->IsExpired() == true)
            {
                // Adjust controllers
                wheels->SetMode(SingleLine_Race_Turn);

                // Transition
                recover = state = QualiState::Turn;
            }
            break;
        }
        case Turn:
        {
            // Speed
            motor->SetDutyCycle(CAR_SPEED_TURN);

            // Direction
            //wheels->SetLine(lineSensor->GetFrontLine(), 0);

            // Transition
            if (lineSensor->GetTrackType() == TrackType::Acceleration)
            {
                recover = state = QualiState::Accelerate;
                motor->SetDutyCycle(CAR_SPEED_STRAIGHT - 0.02f);
                wheels->SetMode(SingleLine_Race_Accel);
                delayDistance->Wait_m(CAR_WAIT_BEFORE_ACCEL);
                //delayTime->Wait_ms(500);
            }
            break;
        }
        case Accelerate:
        {
            // Wait distance
            if (delayDistance->IsExpired() == true)
            //if (delayTime->IsExpired() == true)
            {
                // Adjust controllers
                wheels->SetMode(DualLine_Race_Straight);

                // Transition
                recover = state = QualiState::Straight;
            }
            break;
        }
        case Follow:
        {
            FollowStateMachine();
            break;
        }
        case Stop:
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

    wheels->SetLine(lineSensor->GetFrontLine(), 0);
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

    wheels->SetMode(SingleLineFollow_Slow);

    state   = Turn; // Wait
    recover = Turn;

    dist_ctrl = new Pd_Controller(CAR_DIST_CTRL_P, CAR_DIST_CTRL_D);
    dist_ctrl->SetSetpoint(0.6);

    prescaler = 0;
}

void Car::CheckDeadmanSwitch()
{
    if (remote->GetValue(RemoteChannel::ThrottleCh) < 0.1f)
    {
        state = QualiState::Stop;
    }
    else
    {
        state = recover;
    }
}

void Car::FollowStateMachine()
{
    float dst = distance->GetDistance(ToF_Front);

    if (dst < 4.0f && dst > 0.0f)
    {
        // Speed.
        if (dst > 0.6f)
        {
            motor->SetDutyCycle(0.15f);
        }
        else if (dst < 0.4f)
        {
            if (encoder->GetSpeed() > 0)
            {
                motor->SetDutyCycle(-0.1f);
            }
            else
            {
                motor->SetDutyCycle(0.0f);
            }
        }
        else
        {
            float d = (dst - 0.4) / 0.2 * 0.15f;

            motor->SetDutyCycle(d);
        }
    }

//    if (prescaler == 2)
//    {
//        dist_ctrl->Process(distance->GetDistance(ToF_Front));
//        float speed = dist_ctrl->GetControlValue();
//        if (speed > 0.20f)
//        {
//            speed = 0.20f;
//        }
//        else if (speed < -0.1f)
//        {
//            speed = -0.1f;
//        }
//        else {}
//        motor->SetDutyCycle(speed);
//
//        prescaler = 0;
//    }
//    prescaler++;


    // Direction.
    wheels->SetLine(lineSensor->GetFrontLine(), 0);
    distance->SetFrontServo(wheels->GetFrontAngle()/2);
}

