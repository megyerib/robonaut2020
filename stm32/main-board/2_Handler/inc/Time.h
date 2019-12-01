#pragma once

#include <stdint.h>
#include "stm32f4xx_hal.h"

class Time
{
public:
	static Time* GetInstance();
	uint32_t GetTime(); /* us */

private:
	TIM_HandleTypeDef handle;

	Time();
	void InitTimer();
};
