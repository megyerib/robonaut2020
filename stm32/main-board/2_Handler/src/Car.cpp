#include "Car.h"

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
            motor->SetDutyCycle(0.2f);

            // Direction
            wheels->SetLine(lineSensor->GetFrontLine(), 0);

            // Transition
            if (lineSensor->GetTrackType() == TrackType::Braking)
            {
                state = QualiState::Decelerate;
                delayDistance->Wait_m(1.0f);
            }
            break;
        }
        case Decelerate:
        {
            // Wait distance
            if (delayDistance->IsExpired() == true)
            {
                // Adjust controllers
                wheels->SetMode(SingleLineFollow_Slow);

                // Transition
                state = QualiState::Turn;
            }
            break;
        }
        case Turn:
        {
            // Speed
            motor->SetDutyCycle(0.1f);

            // Direction
            wheels->SetLine(lineSensor->GetFrontLine(), 0);

            // Transition
            if (lineSensor->GetTrackType() == TrackType::Acceleration)
            {
                state = QualiState::Accelerate;
                delayDistance->Wait_m(0.05f);
            }
            break;
        }
        case Accelerate:
        {
            // Wait distance
            if (delayDistance->IsExpired() == true)
            {
                // Adjust controllers
                wheels->SetMode(SingleLineFollow_Fast);

                // Transition
                state = QualiState::Straight;
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
            break;
        }
        default:
        {
            break;
        }
    }
}

void Car::SetSteeringMode(SteeringMode mode)
{
    wheels->SetMode(mode);
}

Car::Car()
{
    radio         = Starter::GetInstance();
    remote        = Remote::GetInstance();
    wheels        = Steering::GetInstance();
    motor         = Traction::GetInstance();
    lineSensor    = TrackDetector::GetInstance();
    distance      = Distance::GetInstance();
    clock         = Timepiece::GetInstance();
   // trace         = Trace::GetInstance();
    delayDistance = new WaitDistance();

    wheels->SetMode(SingleLineFollow_Slow);

    state   = Wait;
    recover = Follow;
}

void Car::CheckDeadmanSwitch()
{
    // TODO !!!!!
    //if (remote->GetValue(RemoteChannel::ThrottleCh) < 0.4f)
    {
        state = QualiState::Stop;
    }
    //else
    //{
        state = recover;
    //}
}

void Car::FollowStateMachine()
{
    // Speed.
    if (distance->GetDistance(ToF_Front) > 0.3f)
    {
        motor->SetDutyCycle(0.0f);
    }
    else if (distance->GetDistance(ToF_Front) < 0.25f)
    {
        motor->SetDutyCycle(0.0f);
    }
    else
    {
        motor->SetDutyCycle(0.0f);
    }

    // Direction.
    wheels->SetLine(lineSensor->GetFrontLine(), 0);
}

