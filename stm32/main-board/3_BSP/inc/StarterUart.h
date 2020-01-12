#pragma once

#include "stm32f4xx_hal.h"

class StarterUart
{
public:
	static StarterUart* GetInstance();

	uint8_t GetLatestChar();

private:
	uint8_t latestChar = 0;

	UART_HandleTypeDef handle;

	StarterUart();

	void InitUart();
	void InitGpio();
};
