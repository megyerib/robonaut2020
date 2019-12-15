#include "Distance.h"
#include "Defines.h"

Distance* Distance::GetInstance()
{
    static Distance instance;
    return &instance;
}

float Distance::GetDistance(DistanceSensor sensor)
{
    float distance = 0.0f;

    switch (sensor)
    {
        case ToF_Front:
        {
            distance = (float)(tof_front->GetDistance_mm()) / 1000.0f;
            break;
        }
        default:
        {
            break;
        }
    }
    return distance;
}

void Distance::SetFrontServo(float angle)
{
    float offset = PI / 2.0f;
    float servo_angle = angle + offset;

    srv_front->SetSteerAngle(servo_angle);
}

void Distance::Process()
{
    tof_front->Process();
}

Distance::Distance()
{
    srv_front = new Servo(eTIM8, TIM_CHANNEL_1);
    srv_front->Enable();

    tof_front = new TOF_L1(0x20,
                           50,
                           200,
                           TOF_FRONT_XSDN_Port,
                           TOF_FRONT_XSDN_Pin);
    tof_front->Init();

    SetFrontServo(1.57f);
}
