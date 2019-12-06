#pragma once

#include "stm32f4xx_hal.h"

typedef enum
{
	Timer3,

	TimerNum
}
Timer;

typedef enum
{
	RemCh1 = 0,
	RemCh2,
	RemCh3,
	//RemCh4,

	RemChNum
}
RemoteHwChannel;

class RemoteHw
{
public:
	static RemoteHw* GetInstance();
	uint16_t GetPulseWidth(RemoteHwChannel ch); /* us */
	static void TimerIrqHandler(Timer t);
	static void InputCaptureCallback(TIM_HandleTypeDef *htim);
	void HandleInputCapture();

private:
	uint16_t lastCapture[RemChNum] = {0};
	uint16_t lastPeriod[RemChNum]  = {0};
	uint16_t lastPulse[RemChNum]   = {0};

	static RemoteHw* instances[TimerNum];
	TIM_HandleTypeDef handle;

	RemoteHw();
	void TimerInit();
};
