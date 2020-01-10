#include "dma.h"
#include <string.h>
#include "stm32f4xx_hal.h"

#define RX_BUF_SIZE    (1024u)

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;

static uint8_t rxBuf[RX_BUF_SIZE];
static uint8_t txBuf[RX_BUF_SIZE] = "Hello!\n";

void dma_uart_init()
{
	HAL_UART_Receive_DMA(&huart2, rxBuf, RX_BUF_SIZE);

	huart2.Instance->CR1 |= USART_CR1_IDLEIE;

	huart2.Instance->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_RXNEIE); // Disable unnecessary interrupts
	// Transfer complete interrupt is necessary
}

void dma()
{
	HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(&huart2, txBuf, strlen((char*)txBuf));

	(void) status;

	HAL_Delay(1000);
}

void dma_check_idle()
{
	static int begin = 0;
	static int end = 0;
	static char buf[RX_BUF_SIZE];
	static int bufSize = 0;

	if (huart2.Instance->SR & USART_SR_IDLE)
	{
		end = RX_BUF_SIZE - hdma_usart2_rx.Instance->NDTR;

		// The Rx line can go idle during the transmission.
		// Waiting for line feed character.
		if (rxBuf[(end-1)%RX_BUF_SIZE] == '\n')
		{
			if (begin <= end)
			{
				int size = end - begin;
				strncpy(buf, (char*) &rxBuf[begin], size);

				bufSize = size;
			}
			else
			{
				// Circular copy
				int size1 = RX_BUF_SIZE - begin;
				int size2 = end;

				strncpy(buf, (char*) &rxBuf[begin], size1);
				strncpy(&buf[size1], (char*) rxBuf, size2);

				bufSize = size1 + size2;
			}

			begin = end;

			// Do something with the data
			{
				memcpy(txBuf, buf, bufSize);
				txBuf[bufSize] = (uint8_t) '\0';
			}
		}

		huart2.Instance->SR &= ~USART_SR_IDLE; // Clear idle flag
		(void) huart2.Instance->DR; // Dummy read to clear idle flag
	}
}
