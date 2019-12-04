#pragma once

#include <stdint.h>
#include "stm32f4xx_hal.h"

class Timepiece
{
public:
	static Timepiece* GetInstance();
	uint32_t GetTime(); /* us */

private:
	TIM_HandleTypeDef handle;

	Timepiece();
	void Init();
};
