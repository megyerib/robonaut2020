#pragma once
#include "stm32f4xx_hal.h"

class DmaUart2
{
public:
	static DmaUart2* GetInstance();
	void Send(void* data, size_t size);

private:
	UART_HandleTypeDef* huart2;
	DMA_HandleTypeDef* hdma_usart2_tx;
	DMA_HandleTypeDef* hdma_usart2_rx;

	DmaUart2();
	void InitDma();
	void InitGpio();
	void InitUart();
};
