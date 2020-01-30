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
//!               90deg
//!          60deg  |  120deg
//!              \  |  /
//!               \ | /
//!     0deg_______\|/________180deg
//!   Left end               Right end
void Servo::SetSteerAngle(float rad)
{
    float compareValue;
    float validCompareValue;

    compareValue = (rad - config.Y_intercept) / config.Gradient;

    // Makes sure that the value is in the valid interval
    if(compareValue > config.Deg_max)
    {
        validCompareValue = config.Deg_max;
    }
    else if(compareValue < config.Deg_min)
    {
        validCompareValue = config.Deg_min;
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
