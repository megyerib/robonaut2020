#include "Distance.h"

Distance* Distance::GetInstance()
{
    static Distance instance;
    return &instance;
}

float Distance::GetDistance(DistanceSensor sensor)
{
    return (float)(tof_front->GetDistance_mm()) / 1000.0f;
}

void Distance::SetFrontServo(float angle)
{
    // TODO convert to +-90deg
    srv_front->SetSteerAngle(angle);
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
                           400,
                           200,
                           TOF_FRONT_XSDN_Port,
                           TOF_FRONT_XSDN_Pin);
    tof_front->Init();

    SetFrontServo(1.57f);
}
