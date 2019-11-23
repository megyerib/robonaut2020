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

    config.Deg_min      = 25;
    config.Deg_30       = 49;
    config.Deg_90       = 85;
    config.Deg_150      = 121;
    config.Deg_max      = 145;
    config.Gradient     = ((PI/180.0f) * (90.0f - 30.0f)/(config.Deg_90 - config.Deg_30));
    config.Y_intercept  = ((PI/180.0f) * 90.0f - config.Deg_90 * config.Gradient);

    timer = new Timer(Htim, Channel, 2879, 1249, isChNegated);

    timer->Start_PWM();
}

AnalogServo::~AnalogServo()
{

}



