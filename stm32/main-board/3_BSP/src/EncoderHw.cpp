#include "EncoderHw.h"
#include "stm32f4xx_hal.h"

EncoderHw::EncoderHw()
{
	InitTimer();

	TIM2->CNT = 0; // Reset timer

	HAL_TIM_Encoder_Start(&handle, TIM_CHANNEL_ALL);
}

EncoderHw* EncoderHw::GetInstance()
{
	static EncoderHw instance;
	return &instance;
}

uint32_t EncoderHw::GetCounterValue()
{
	return TIM2->CNT;
}

void EncoderHw::InitTimer()
{
	// GPIO -----------------------------------------------------

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/**TIM2 GPIO Configuration
	PB8     ------> TIM2_CH1
	PB9     ------> TIM2_CH2
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Timer ----------------------------------------------------

	__HAL_RCC_TIM2_CLK_ENABLE();

	TIM_Encoder_InitTypeDef sConfig       = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	handle.Instance               = TIM2;
	handle.Init.Prescaler         = 0;
	handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	handle.Init.Period            = 0xFFFFFFFF;
	handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	sConfig.EncoderMode           = TIM_ENCODERMODE_TI12;

	sConfig.IC1Polarity           = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection          = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler          = TIM_ICPSC_DIV1;
	sConfig.IC1Filter             = 0;

	sConfig.IC2Polarity           = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection          = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler          = TIM_ICPSC_DIV1;
	sConfig.IC2Filter             = 0;

	if (HAL_TIM_Encoder_Init(&handle, &sConfig) != HAL_OK)
	{
		//Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&handle, &sMasterConfig) != HAL_OK)
	{
		//Error_Handler();
	}
}
