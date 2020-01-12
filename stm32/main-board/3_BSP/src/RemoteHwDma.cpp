#include "RemoteHwDma.h"

#define VALID_LOW    500
#define VALID_HIGH  2500

uint16_t RemoteHwDma::data[RemChNum][DATA_BUF_SIZE];

RemoteHwDma::RemoteHwDma()
{
	InitGpio();
	InitTimer();
	InitDma();

	HAL_TIM_IC_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*) data[RemCh1], DATA_BUF_SIZE);
	HAL_TIM_IC_Start_DMA(&htim3, TIM_CHANNEL_2, (uint32_t*) data[RemCh2], DATA_BUF_SIZE);
	HAL_TIM_IC_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t*) data[RemCh3], DATA_BUF_SIZE);
}

RemoteHwDma* RemoteHwDma::GetInstance()
{
	static RemoteHwDma instance;
	return &instance;
}

uint16_t RemoteHwDma::GetPulseWidth(RemoteHwChannel ch)
{
	return 0; // TODO
}

void RemoteHwDma::InitGpio()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/**TIM3 GPIO Configuration
	PA6     ------> TIM3_CH1
	PA7     ------> TIM3_CH2
	PB1     ------> TIM3_CH4
	*/

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void RemoteHwDma::InitTimer()
{
	__HAL_RCC_TIM3_CLK_ENABLE();

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	htim3.Instance                    = TIM3;
	htim3.Init.Prescaler              = 90 - 1;
	htim3.Init.CounterMode            = TIM_COUNTERMODE_UP;
	htim3.Init.Period                 = 0xFFFF;
	htim3.Init.ClockDivision          = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload      = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_IC_Init(&htim3);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

	sConfigIC.ICPolarity              = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection             = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler             = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter                = 0;

	HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1);
	HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2);
	HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_4);
}

void RemoteHwDma::InitDma()
{
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* TIM3 DMA Init */
	/* TIM3_CH1_TRIG Init */
	hdma_tim3_ch1_trig.Instance = DMA1_Stream4;
	hdma_tim3_ch1_trig.Init.Channel = DMA_CHANNEL_5;
	hdma_tim3_ch1_trig.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim3_ch1_trig.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim3_ch1_trig.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim3_ch1_trig.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim3_ch1_trig.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim3_ch1_trig.Init.Mode = DMA_CIRCULAR;
	hdma_tim3_ch1_trig.Init.Priority = DMA_PRIORITY_LOW;
	hdma_tim3_ch1_trig.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma_tim3_ch1_trig);

	/* Several peripheral DMA handle pointers point to the same DMA handle.
	Be aware that there is only one stream to perform all the requested DMAs. */
	__HAL_LINKDMA(&htim3,hdma[TIM_DMA_ID_CC1],hdma_tim3_ch1_trig);
	__HAL_LINKDMA(&htim3,hdma[TIM_DMA_ID_TRIGGER],hdma_tim3_ch1_trig);

	/* TIM3_CH2 Init */
	hdma_tim3_ch2.Instance = DMA1_Stream5;
	hdma_tim3_ch2.Init.Channel = DMA_CHANNEL_5;
	hdma_tim3_ch2.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim3_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim3_ch2.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim3_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim3_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim3_ch2.Init.Mode = DMA_CIRCULAR;
	hdma_tim3_ch2.Init.Priority = DMA_PRIORITY_LOW;
	hdma_tim3_ch2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma_tim3_ch2);

	__HAL_LINKDMA(&htim3,hdma[TIM_DMA_ID_CC2],hdma_tim3_ch2);

	/* TIM3_CH4_UP Init */
	hdma_tim3_ch4_up.Instance = DMA1_Stream2;
	hdma_tim3_ch4_up.Init.Channel = DMA_CHANNEL_5;
	hdma_tim3_ch4_up.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_tim3_ch4_up.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim3_ch4_up.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim3_ch4_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim3_ch4_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_tim3_ch4_up.Init.Mode = DMA_CIRCULAR;
	hdma_tim3_ch4_up.Init.Priority = DMA_PRIORITY_LOW;
	hdma_tim3_ch4_up.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma_tim3_ch4_up);

	/* Several peripheral DMA handle pointers point to the same DMA handle.
	Be aware that there is only one stream to perform all the requested DMAs. */
	__HAL_LINKDMA(&htim3,hdma[TIM_DMA_ID_CC4],hdma_tim3_ch4_up);
	__HAL_LINKDMA(&htim3,hdma[TIM_DMA_ID_UPDATE],hdma_tim3_ch4_up);


	HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);

	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void RemoteHwDma::timCh1Irq()
{
	HAL_DMA_IRQHandler(&hdma_tim3_ch1_trig);
}

void RemoteHwDma::timCh2Irq()
{
	HAL_DMA_IRQHandler(&hdma_tim3_ch2);
}

void RemoteHwDma::timCh4Irq()
{
	HAL_DMA_IRQHandler(&hdma_tim3_ch4_up);
}

extern "C" void DMA1_Stream2_IRQHandler(void)
{
	RemoteHwDma::GetInstance()->timCh4Irq();
}

extern "C" void DMA1_Stream4_IRQHandler(void)
{
	RemoteHwDma::GetInstance()->timCh1Irq();
}

extern "C" void DMA1_Stream5_IRQHandler(void)
{
	RemoteHwDma::GetInstance()->timCh2Irq();
}
