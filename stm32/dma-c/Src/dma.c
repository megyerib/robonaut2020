#include "dma.h"
#include <string.h>
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;

static uint8_t rxBuf[50];

void dma_uart_init()
{
	HAL_UART_Receive_DMA(&huart2, rxBuf, 50);

	huart2.Instance->CR1 |= USART_CR1_IDLEIE;
}

void dma()
{
	static uint8_t msg[] = "Hello!\n";

	HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(&huart2, msg, strlen((char*)msg));

	(void) status;

	HAL_Delay(1000);
}

void dma_check_idle()
{
	static int begin = 0;
	static int end = 0;
	static char buf[50];

	if (huart2.Instance->SR & USART_SR_IDLE)
	{
		end = 50 - hdma_usart2_rx.Instance->NDTR;

		if (begin < end)
		{
			int size = end - begin;
			strncpy(buf, &rxBuf[begin], size);
		}
		else
		{
			// TODO circular copy
		}

		begin = end;
	}

	// TODO this flag only clears when I look after it in the SFRs tab.
	// This bug only occurs until the first successful clear. FML
	// K, I'm out for today.
	// https://electronics.stackexchange.com/questions/222638/clearing-usart-uart-interrupt-flags-in-an-stm32

	huart2.Instance->SR &= ~USART_SR_IDLE; // Clear idle flag

}
