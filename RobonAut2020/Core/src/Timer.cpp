/*
 * Timer.cpp
 *
 *  Created on: 2019. nov. 16.
 *      Author: Joci
 */

#include "Timer.h"

Timer::Timer(TIM_HandleTypeDef* Htim, U32 Channel, U16 Prescaler, U16 Period, bool Negated)
{
    negated = Negated;
    htim = Htim;

    if (isChannelValid(Channel) == true)
    {
        channel = Channel;
    }
    else
    {
        channel = TIM_CHANNEL_1;
    }

    timCfg.Prescaler = Prescaler;
    htim->Init.Prescaler = Prescaler;


    timCfg.Period    = Period;
    htim->Init.Period = Period;

    if (htim->Instance == TIM8)
    {
        timCfg.Frequency = 50;
        timCfg.TimerClk  = 62500;
    }
    else if (htim->Instance == TIM12)
    {
        timCfg.Frequency = 250;
        timCfg.TimerClk  = 62500;
    }
    else if (htim->Instance == TIM2)
    {
        timCfg.Frequency = 0;
        timCfg.TimerClk  = 0;
    }
    else if (htim->Instance == TIM3)
    {
        timCfg.Frequency = 0;
        timCfg.TimerClk  = 0;
    }
    else
    {
        timCfg.Frequency = 0;
        timCfg.TimerClk  = 0;
    }

    Callback = NULL;
}

void Timer::SetTimerCompareValue(U32 compareValue)
{
    U32 validCompareValue = SaturateCompareValue(compareValue);

    __HAL_TIM_SET_COMPARE(htim, channel, validCompareValue);
}

U32 Timer::GetTimerCompareValue(void)
{
    return __HAL_TIM_GET_COMPARE(htim, channel);
}

void Timer::RegisterCallback(void (*callback)(TIM_HandleTypeDef*))
{
   Callback = callback;
}

bool Timer::isChannelValid(U32 Channel)
{
    bool channelIsValid = false;

    if ((Channel == TIM_CHANNEL_1) ||
        (Channel == TIM_CHANNEL_2) ||
        (Channel == TIM_CHANNEL_3) ||
        (Channel == TIM_CHANNEL_4) ||
        (Channel == TIM_CHANNEL_ALL))
    {
        channelIsValid = true;
    }

    return channelIsValid;
}

U32 Timer::SaturateCompareValue(U32 compare)
{
    U32 validCompareValue;

    if(compare > timCfg.Period)
    {
        validCompareValue = timCfg.Period;
    }
    else if(compare < 0)
    {
        validCompareValue = 0;
    }
    else
    {
        validCompareValue = compare;
    }

    return validCompareValue;
}

void Timer::Start_PWM()
{
    if (negated == true)
    {
         HAL_TIMEx_PWMN_Start(htim, channel);
    }
    else
    {
        HAL_TIM_PWM_Start(htim, channel);
    }
}


void Timer::Stop_PWM()
{
    if (negated == true)
    {
        HAL_TIMEx_PWMN_Stop(htim, channel);
    }
    else
    {
        HAL_TIM_PWM_Stop(htim, channel);
    }
}

