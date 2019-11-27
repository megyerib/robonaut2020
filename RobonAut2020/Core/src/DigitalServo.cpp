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

    config.Rad_min      = 62;
    config.Rad_30deg    = 76;
    config.Rad_90deg    = 90;
    config.Rad_150deg   = 114;
    config.Rad_max      = 118;
    config.Gradient     = ((PI/180.0f) * (90.0f - 30.0f)/(config.Rad_90deg - config.Rad_30deg));
    config.Y_intercept  = ((PI/180.0f) * (90.0f - config.Rad_90deg * config.Gradient));


    timer = new Timer(Htim, Channel, 1439U, 249, false);
    timer->Start_PWM();
}

DigitalServo::~DigitalServo()
{

}

