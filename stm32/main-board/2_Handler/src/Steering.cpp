#include "Steering.h"
#include "Defines.h"

Steering* Steering::GetInstance()
{
    static Steering instance;
    return &instance;
}

void Steering::SetMode(SteeringMode mode)
{
    this->mode = mode;
}

void Steering::SetLine(float front_line, float rear_line)
{

}

//! Steering wheel:
//!
//!                 0 rad
//!        0.52 rad   |   -0.52 rad
//!               \   |   /
//!                \  |  /
//!                 \ | /
//!  1.57 rad________\|/________-1.57 rad
//!  Left end                   Right end
void Steering::SetAngle(float front_angle, float rear_angle)
{
    float offset        = PI/2.0f;
    float scale         = 1.0f;
    float servo_angle;

    servo_angle = (front_angle + offset) * scale;



    srv_front->SetSteerAngle(servo_angle);
}

void Steering::Process()
{
    switch (mode)
    {
        case DualLineFollow:
        {

            break;
        }
        case SingleLineFollow:
        {

            break;
        }
        case Free:
        {

            break;
        }
        case Reverse:
        {

            break;
        }
        default:
        {

            break;
        }
    }
}

Steering::Steering()
{
    srv_front = new Servo(eTIM12, TIM_CHANNEL_1);
    srv_rear  = new Servo(eTIM12, TIM_CHANNEL_2);
}
