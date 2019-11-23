/*
 * DigitalServo.cpp
 *
 *  Created on: 2019. nov. 16.
 *      Author: Joci
 */

#include "DigitalServo.h"

DigitalServo::DigitalServo(TIM_HandleTypeDef* Htim, U32 Channel, ServoType Type, bool isChNegated)
{
    type    = Type;
    enabled = false;

    config.Deg_min      = 62;
    config.Deg_30       = 76;
    config.Deg_90       = 90;
    config.Deg_150      = 114;
    config.Deg_max      = 118;
    config.Gradient     = ((PI/180.0f) * (90.0f - 30.0f)/(config.Deg_90 - config.Deg_30));
    config.Y_intercept  = ((PI/180.0f) * (90.0f - config.Deg_90 * config.Gradient));


    timer = new Timer(Htim, Channel, 1439U, 249, false);
    timer->Start_PWM();
}

DigitalServo::~DigitalServo()
{

}

