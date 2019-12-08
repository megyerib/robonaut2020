#include "RemoteHw.h"
#include "NvicPrio.h"

RemoteHw* RemoteHw::instances[TimerNum] = {nullptr};

const uint32_t timerChannels[4] =
{
	TIM_CHANNEL_1,
	TIM_CHANNEL_2,
	TIM_CHANNEL_3,
	TIM_CHANNEL_4
};

RemoteHw::RemoteHw()
{
	TimerInit();

	instances[Timer3] = this;

	for (int i = 0; i < RemChNum; i++)
	{
		HAL_TIM_IC_Start_IT(&handle, timerChannels[i]);
	}
}

RemoteHw* RemoteHw::GetInstance()
{
	static RemoteHw instance;
	return &instance;
}

uint16_t RemoteHw::GetPulseWidth(RemoteHwChannel ch)
{
	return lastPulse[ch];
}

void RemoteHw::TimerInit()
{
	// GPIO --------------------------------------------------------------------

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**TIM3 GPIO Configuration
	PA6     ------> TIM3_CH1
	PA7     ------> TIM3_CH2
	PB0     ------> TIM3_CH3
	PB1     ------> TIM3_CH4
	*/
	GPIO_InitStruct.Pin       = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;           // They work when pulled down
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin       = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;           // Don't work when pulled down
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Timer -------------------------------------------------------------------

	/* TIM3 clock enable */
	__HAL_RCC_TIM3_CLK_ENABLE();

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	handle.Instance = TIM3;
	handle.Init.Prescaler = 90-1;
	handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	handle.Init.Period = 0xFFFF;
	handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_IC_Init(&handle) != HAL_OK)
	{
		//Error_Handler();
	}

	// Master-slave (TODO needed?) ---------------------------------------------

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&handle, &sMasterConfig) != HAL_OK)
	{
		//Error_Handler();
	}

	// Channels ----------------------------------------------------------------

	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;

	for (int i = 0; i < RemChNum; i++)
	{
		HAL_TIM_IC_ConfigChannel(&handle, &sConfigIC, timerChannels[i]);
	}

	// NVIC --------------------------------------------------------------------

	HAL_NVIC_SetPriority(TIM3_IRQn, REMOTE_NVIC_PRIO, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void RemoteHw::TimerIrqHandler(Timer t)
{
	RemoteHw* remoteInstance = instances[Timer3];

	if (remoteInstance != nullptr)
	{
		HAL_TIM_IRQHandler(&remoteInstance->handle);
	}
}

void RemoteHw::InputCaptureCallback(TIM_HandleTypeDef *htim)
{
	RemoteHw* remoteInstance = instances[Timer3];

	if (remoteInstance != nullptr)
	{
		remoteInstance->HandleInputCapture();
	}
}

void RemoteHw::HandleInputCapture()
{
	for (int i = 0; i < RemChNum; i++)
	{
		uint16_t capture = __HAL_TIM_GET_COMPARE(&handle, timerChannels[i]);

		if (capture != lastCapture[i])
		{
			uint16_t period = capture - lastCapture[i];

			if (period < lastPeriod[i])
			{
				lastPulse[i] = period;
			}

			lastPeriod[i]  = period;
			lastCapture[i] = capture;
		}
	}
}

// ---------------------------------------------------------------------------------------------------------------------

extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	RemoteHw::InputCaptureCallback(htim); // Can be changed in a more generalized sw
}

extern "C" void TIM3_IRQHandler(void)
{
	RemoteHw::TimerIrqHandler(Timer3);
}

extern "C" void WWDG_IRQHandler(void)
{
	// To shut the MCU up
}
