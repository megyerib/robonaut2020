/*
 * Timer.h
 *
 *  Created on: 2019. nov. 16.
 *      Author: Joci
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "OVERRIDE_Types.h"
#include "tim.h"

struct TimerConfig
{
    U16 Prescaler;
    U16 Period;
    U8  Frequency;
    U16 TimerClk;
};

class Timer
{
private:
    bool               negated;
    TimerConfig        timCfg;
    TIM_HandleTypeDef* htim;
    U32                channel;

    void (*Callback)(TIM_HandleTypeDef*);

    bool isChannelValid(U32 Channel);
    U32  SaturateCompareValue(U32 compare);

public:
    Timer(TIM_HandleTypeDef* Htim, U32 Channel, U16 Prescaler, U16 Period, bool Negated);

    void SetTimerCompareValue(U32 compareValue);
    U32  GetTimerCompareValue(void);

    void RegisterCallback(void (*callback)(TIM_HandleTypeDef*));

    void Start_PWM();
    void Stop_PWM();
};

#endif /* TIMER_H_ */
