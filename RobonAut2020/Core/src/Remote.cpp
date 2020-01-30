/*
 * Remote.cpp
 *
 *  Created on: Nov 16, 2019
 *      Author: Joci
 */

#include "Remote.h"

U8 Remote::State = 0;

Remote::Remote(void)
{
    timCh1 = new Timer(&htim3, TIM_CHANNEL_1, 89, 0xffff, false);
    timCh2 = new Timer(&htim3, TIM_CHANNEL_2, 89, 0xffff, false);
    timCh3 = new Timer(&htim3, TIM_CHANNEL_3, 89, 0xffff, false);
    timCh4 = new Timer(&htim3, TIM_CHANNEL_4, 89, 0xffff, false);
}

U8 Remote::GetState(U32 Channel)
{
    return State;
}

void Remote::IC_IT(TIM_HandleTypeDef *htim)
{
    U32 inputCh1 = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1);
    U32 inputCh2 = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_2);
    U32 inputCh3 = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_3);
    U32 inputCh4 = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_4);

    Remote::State = inputCh1 + inputCh2 + inputCh3 + inputCh4;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        Remote::IC_IT(htim);
    }
}
