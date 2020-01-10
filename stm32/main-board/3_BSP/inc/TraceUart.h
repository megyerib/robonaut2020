#pragma once

#include "stm32f4xx_hal.h"

#define RX_BUF_SIZE (1024u)

class TraceUart
{
public:
	static TraceUart* GetInstance();
	void Send(void* buf, size_t size);
	void Receive(void* buf, size_t* size);

	static void HandleUartIrq();
	static void HandleDmaRxIrq();
	static void HandleDmaTxIrq();

private:
	static uint8_t rxBuf[RX_BUF_SIZE];
	int rxBufIndex = 0;

	static TraceUart* instance;

	UART_HandleTypeDef huart2;
	DMA_HandleTypeDef  hdma_usart2_tx;
	DMA_HandleTypeDef  hdma_usart2_rx;

	TraceUart();
	void InitHW();
};
