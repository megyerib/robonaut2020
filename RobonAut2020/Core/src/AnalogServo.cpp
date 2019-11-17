/*
 * AnalogServo.cpp
 *
 *  Created on: 2019. nov. 16.
 *      Author: Joci
 */

#include "AnalogServo.h"

AnalogServo::AnalogServo(TIM_HandleTypeDef* Htim, U32 Channel, ServoType Type, bool isChNegated)
{
    type    = Type;
    enabled = false;

    config.Deg_min      = 68;
    config.Deg_30       = 74;
    config.Deg_90       = 91;
    config.Deg_150      = 113;
    config.Deg_max      = 114;
    config.Gradient     = ((PI/180.0f) * (90.0f - 30.0f)/(config.Deg_90 - config.Deg_30));
    config.Y_intercept  = ((PI/180.0f) * (90.0f - config.Deg_90 * config.Gradient));

    timer = new Timer(Htim, Channel, 1439, 1249, isChNegated);

    timer->Start_PWM();
}

AnalogServo::~AnalogServo()
{

}



