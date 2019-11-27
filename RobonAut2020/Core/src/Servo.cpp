/*
 * Servo.cpp
 *
 *  Created on: 2019. nov. 11.
 *      Author: Joci
 */

#include "Servo.h"

Servo::Servo()
{

}

Servo::~Servo()
{

}

void Servo::Enable(void)
{
    enabled = true;
}

void Servo::Disable(void)
{
    enabled = false;
}

//! Servo Anlge:
//!                 1.57 rad
//!          2.09 rad  |  0.52 rad
//!                 \  |  /
//!                  \ | /
//!    3.14 rad_______\|/________0 rad
//!    Left end                 Right end
void Servo::SetSteerAngle(float rad)
{
    float compareValue;
    float validCompareValue;

    compareValue = (rad - config.Y_intercept) / config.Gradient;

    // Makes sure that the value is in the valid interval
    if(compareValue > config.Rad_max)
    {
        validCompareValue = config.Rad_max;
    }
    else if(compareValue < config.Rad_min)
    {
        validCompareValue = config.Rad_min;
    }
    else
    {
        validCompareValue = compareValue;
    }

    timer->SetTimerCompareValue(validCompareValue);
}

float Servo::GetSteerAngle(void)
{
    float radian;
    float compareValue;

    compareValue = timer->GetTimerCompareValue();
    radian = config.Gradient * compareValue + config.Y_intercept;

    return radian;
}

void Servo::SetCalibration(U8 min,
                           U8 rad30,
                           U8 rad90,
                           U8 rad150,
                           U8 max)
{
    config.Rad_min    = min;
    config.Rad_30deg  = rad30;
    config.Rad_90deg  = rad90;
    config.Rad_150deg = rad150;
    config.Rad_max    = max;
}
