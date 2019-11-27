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

    config.Rad_min      = 25;
    config.Rad_30deg    = 49;
    config.Rad_90deg    = 85;
    config.Rad_150deg   = 121;
    config.Rad_max      = 145;
    config.Gradient     = ((PI/180.0f) * (90.0f - 30.0f)/(config.Rad_90deg - config.Rad_30deg));
    config.Y_intercept  = ((PI/180.0f) * 90.0f - config.Rad_90deg * config.Gradient);

    timer = new Timer(Htim, Channel, 2879, 1249, isChNegated);

    timer->Start_PWM();
}

AnalogServo::~AnalogServo()
{

}



