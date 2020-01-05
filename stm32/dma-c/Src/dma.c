#include "dma.h"
#include <string.h>
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;

void dma()
{
	static uint8_t msg[] = "Hello!\n";

	HAL_UART_Transmit_DMA(&huart2, msg, strlen((char*)msg));

	HAL_Delay(1000);
}
