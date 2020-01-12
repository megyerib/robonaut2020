#pragma once

#include "stm32f4xx_hal.h"

#define DATA_BUF_SIZE  (20u)

typedef enum
{
	RemCh1 = 0,
	RemCh2,
	RemCh3,

	RemChNum
}
RemoteHwChannel;

class RemoteHwDma
{
public:
	static RemoteHwDma* GetInstance();
	uint16_t GetPulseWidth(RemoteHwChannel ch); /* us */

	void timCh1Irq();
	void timCh2Irq();
	void timCh4Irq();

private:
	static uint16_t data[RemChNum][DATA_BUF_SIZE];

	TIM_HandleTypeDef htim3;

	DMA_HandleTypeDef hdma_tim3_ch1_trig;
	DMA_HandleTypeDef hdma_tim3_ch2;
	DMA_HandleTypeDef hdma_tim3_ch4_up;

	RemoteHwDma();

	void InitGpio();
	void InitTimer();
	void InitDma();
};
