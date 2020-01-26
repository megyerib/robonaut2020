#include <DmaUart.h>
#include <string.h>

DmaUart::DmaUart(DMA_UART_CFG& cfg) : cfg(cfg)
{
	InitGpio();
	InitUart();
	InitDma();

	HAL_UART_Receive_DMA(&huart, cfg.rxBuf, cfg.rxBufSize);
}

int32_t DmaUart::Transmit(const void* buffer, size_t size)
{
	HAL_UART_Transmit_DMA(&huart, (uint8_t*) buffer, size);

	return 0;
}

int32_t DmaUart::Receive(void* buffer, size_t& size, size_t targetSize)
{
	UNUSED(targetSize); // TODO

	uint8_t* buf = (uint8_t*) buffer;
	size_t end;

	size = 0;

	end = cfg.rxBufSize - hdma_uart_rx.Instance->NDTR;

	if (rxBufIndex <= end)
	{
		size = end - rxBufIndex;
		memcpy(buf, &cfg.rxBuf[rxBufIndex], size);
	}
	else // Circular copy
	{
		int size1 = cfg.rxBufSize - rxBufIndex;
		int size2 = end;

		memcpy(buf, &cfg.rxBuf[rxBufIndex], size1);
		memcpy(&buf[size1], cfg.rxBuf, size2);

		size = size1 + size2;
	}

	rxBufIndex = end;

	return size;
}

void DmaUart::HandleUartIrq()
{
	HAL_UART_IRQHandler(&huart);
}

void DmaUart::HandleDmaRxIrq()
{
	HAL_DMA_IRQHandler(&hdma_uart_rx);
}

void DmaUart::HandleDmaTxIrq()
{
	HAL_DMA_IRQHandler(&hdma_uart_tx);
}

// UART: USART2
void DmaUart::InitUart()
{
	cfg.uartClkEn();

	huart.Instance          = cfg.uartInstance;
	huart.Init.BaudRate     = cfg.uartBaudRate;
	huart.Init.WordLength   = UART_WORDLENGTH_8B;
	huart.Init.StopBits     = UART_STOPBITS_1;
	huart.Init.Parity       = UART_PARITY_NONE;
	huart.Init.Mode         = UART_MODE_TX_RX;
	huart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&huart);

	// Interrupts
	HAL_NVIC_SetPriority(cfg.uartIrq, 0, 0);
	HAL_NVIC_EnableIRQ(cfg.uartIrq);

	// Disable unnecessary interrupts
	huart.Instance->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_RXNEIE);
	// Transfer complete interrupt is necessary
}

void DmaUart::InitGpio()
{
	cfg.gpioTxClkEn();
	cfg.gpioRxClkEn();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin       = cfg.gpioTxPin;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = cfg.gpioAf;

	if (cfg.txEnabled)
	{
		HAL_GPIO_Init(cfg.gpioTxPort, &GPIO_InitStruct);
	}

	GPIO_InitStruct.Pin       = cfg.gpioRxPin;

	if (cfg.rxEnabled)
	{
		HAL_GPIO_Init(cfg.gpioRxPort, &GPIO_InitStruct);
	}
}

void DmaUart::InitDma()
{
	cfg.dmaClkEn();

	// USART TX Init

	hdma_uart_tx.Instance                 = cfg.dmaTxStream;
	hdma_uart_tx.Init.Channel             = cfg.dmaTxChannel;
	hdma_uart_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	hdma_uart_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_uart_tx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_uart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_uart_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_uart_tx.Init.Mode                = DMA_NORMAL;
	hdma_uart_tx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_uart_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

	if (cfg.txEnabled)
	{
		HAL_DMA_Init(&hdma_uart_tx);
		__HAL_LINKDMA(&huart, hdmatx, hdma_uart_tx);

		HAL_NVIC_SetPriority(cfg.dmaTxIrq, 0, 0);
		HAL_NVIC_EnableIRQ(cfg.dmaTxIrq);
	}

	// USART RX Init

	hdma_uart_rx.Instance                 = cfg.dmaRxStream;
	hdma_uart_rx.Init.Channel             = cfg.dmaRxChannel;
	hdma_uart_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	hdma_uart_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	hdma_uart_rx.Init.MemInc              = DMA_MINC_ENABLE;
	hdma_uart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_uart_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	hdma_uart_rx.Init.Mode                = DMA_CIRCULAR;
	hdma_uart_rx.Init.Priority            = DMA_PRIORITY_LOW;
	hdma_uart_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;

	if (cfg.rxEnabled)
	{
		HAL_DMA_Init(&hdma_uart_rx);
		__HAL_LINKDMA(&huart, hdmarx, hdma_uart_rx);

		HAL_NVIC_SetPriority(cfg.dmaRxIrq, 0, 0);
		HAL_NVIC_EnableIRQ(cfg.dmaRxIrq);
	}
}
