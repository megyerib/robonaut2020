#include "DmaUart2.h"
#include <cstring> /* memcpy */

DmaUart2* DmaUart2::GetInstance()
{
	static DmaUart2 instance;
	return &instance;
}

DmaUart2::DmaUart2()
{
	huart2 = new UART_HandleTypeDef;
	hdma_usart2_tx = new DMA_HandleTypeDef;
	hdma_usart2_rx = new DMA_HandleTypeDef;

	InitGpio();
	InitDma();
	InitUart();
}

void DmaUart2::InitDma()
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	/* DMA1_Stream6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

	/* USART2_TX Init */
	hdma_usart2_tx->Instance                 = DMA1_Stream6;
	hdma_usart2_tx->Init.Channel             = DMA_CHANNEL_4;
	hdma_usart2_tx->Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_usart2_tx->Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_usart2_tx->Init.MemInc              = DMA_MINC_ENABLE;
	hdma_usart2_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_tx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_usart2_tx->Init.Mode                = DMA_NORMAL;
	hdma_usart2_tx->Init.Priority            = DMA_PRIORITY_LOW;
	hdma_usart2_tx->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(hdma_usart2_tx) != HAL_OK)
	{
		//Error_Handler();
	}

	__HAL_LINKDMA(huart2,hdmatx,*hdma_usart2_tx);

	/* USART2_RX Init */
	hdma_usart2_rx->Instance                 = DMA1_Stream5;
	hdma_usart2_rx->Init.Channel             = DMA_CHANNEL_4;
	hdma_usart2_rx->Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_usart2_rx->Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_usart2_rx->Init.MemInc              = DMA_MINC_ENABLE;
	hdma_usart2_rx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_rx->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_usart2_rx->Init.Mode                = DMA_CIRCULAR;
	hdma_usart2_rx->Init.Priority            = DMA_PRIORITY_LOW;
	hdma_usart2_rx->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(hdma_usart2_rx) != HAL_OK)
	{
		//Error_Handler();
	}

	__HAL_LINKDMA(huart2,hdmarx,*hdma_usart2_rx);
}

void DmaUart2::InitGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**USART2 GPIO Configuration
	PA2     ------> USART2_TX
	PA3     ------> USART2_RX
	*/
	GPIO_InitStruct.Pin       = GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void DmaUart2::InitUart()
{
	__HAL_RCC_USART2_CLK_ENABLE();

	huart2->Instance = USART2;
	huart2->Init.BaudRate = 115200;
	huart2->Init.WordLength = UART_WORDLENGTH_8B;
	huart2->Init.StopBits = UART_STOPBITS_1;
	huart2->Init.Parity = UART_PARITY_NONE;
	huart2->Init.Mode = UART_MODE_TX_RX;
	huart2->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2->Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(huart2) != HAL_OK)
	{
		//Error_Handler();
	}
}

void DmaUart2::Send(void* data, size_t size)
{
	// TODO buffer
	static uint8_t* buf[30];

	memcpy(buf, data, size);
	HAL_UART_Transmit_DMA(huart2, (uint8_t*)data, size);
}
