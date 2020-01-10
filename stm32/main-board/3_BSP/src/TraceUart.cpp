#include "TraceUart.h"
#include <string.h>

TraceUart* TraceUart::instance = nullptr;
uint8_t TraceUart::rxBuf[RX_BUF_SIZE];

TraceUart::TraceUart()
{
	instance = this;

	InitHW();

	HAL_UART_Receive_DMA(&huart2, rxBuf, RX_BUF_SIZE);
}

TraceUart* TraceUart::GetInstance()
{
	static TraceUart i;
	return instance;
}

void TraceUart::Send(void* buf, size_t size)
{
	HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(&huart2, (uint8_t*)buf, size);
	UNUSED(status);
}

void TraceUart::Receive(void* dst, size_t* size)
{
	int end = RX_BUF_SIZE - hdma_usart2_rx.Instance->NDTR;
	char* buf = (char*) dst;

	if (rxBufIndex <= end)
	{
		*size = end - rxBufIndex;
		strncpy(buf, (char*) &rxBuf[rxBufIndex], *size);
	}
	else
	{
		// Circular copy
		int size1 = RX_BUF_SIZE - rxBufIndex;
		int size2 = end;

		strncpy(buf, (char*) &rxBuf[rxBufIndex], size1);
		strncpy(&buf[size1], (char*) rxBuf, size2);

		*size = size1 + size2;
	}

	rxBufIndex = end;
}

void TraceUart::HandleUartIrq()
{
	if (instance == nullptr)
		return;

	HAL_UART_IRQHandler(&instance->huart2);
}

void TraceUart::HandleDmaRxIrq()
{
	if (instance == nullptr)
		return;

	HAL_DMA_IRQHandler(&instance->hdma_usart2_rx);
}

void TraceUart::HandleDmaTxIrq()
{
	if (instance == nullptr)
		return;

	HAL_DMA_IRQHandler(&instance->hdma_usart2_tx);
}

void TraceUart::InitHW()
{
	//  UART: USART2
    //
	//        DMA             GPIO
	//  TX:   DMA1 Stream 6   PA2
	//  RX:   DMA1 Stream 5   PA3

	// 1 GPIO CLK --------------------------------

	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2 DMA CLK ---------------------------------

	__HAL_RCC_DMA1_CLK_ENABLE();

	// 3 DMA IT ---------------------------------- TODO prios

	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

	HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

	// 4 UART ------------------------------------

	__HAL_RCC_USART2_CLK_ENABLE();

	huart2.Instance          = USART2;
	huart2.Init.BaudRate     = 115200;
	huart2.Init.WordLength   = UART_WORDLENGTH_8B;
	huart2.Init.StopBits     = UART_STOPBITS_1;
	huart2.Init.Parity       = UART_PARITY_NONE;
	huart2.Init.Mode         = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart2);

	// 6 UART GPIO -------------------------------

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin       = GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// 7 UART DMA --------------------------------

	/* USART2_TX Init */
	hdma_usart2_tx.Instance                 = DMA1_Stream6;
	hdma_usart2_tx.Init.Channel             = DMA_CHANNEL_4;
	hdma_usart2_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_usart2_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_usart2_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_usart2_tx.Init.Mode                = DMA_NORMAL;
	hdma_usart2_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_usart2_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma_usart2_tx);
	__HAL_LINKDMA(&huart2, hdmatx, hdma_usart2_tx);

	/* USART2_RX Init */
	hdma_usart2_rx.Instance                 = DMA1_Stream5;
	hdma_usart2_rx.Init.Channel             = DMA_CHANNEL_4;
	hdma_usart2_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_usart2_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_usart2_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_usart2_rx.Init.Mode                = DMA_CIRCULAR;
	hdma_usart2_rx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_usart2_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&hdma_usart2_rx);
	__HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx);

	// 8 UART IT ---------------------------------

	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

	// Disable unnecessary interrupts
	// Transfer complete interrupt is necessary
	huart2.Instance->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_RXNEIE);
}

extern "C" void DMA1_Stream5_IRQHandler(void)
{
	TraceUart::HandleDmaRxIrq();
}

extern "C" void DMA1_Stream6_IRQHandler(void)
{
	TraceUart::HandleDmaTxIrq();
}

extern "C" void USART2_IRQHandler(void)
{
	TraceUart::HandleUartIrq();
}
