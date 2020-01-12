#include "StarterUart.h"
#include <cstring>
#include "NvicPrio.h"

StarterUart::StarterUart()
{
	InitGpio();
	InitUart();
}

StarterUart* StarterUart::GetInstance()
{
	static StarterUart instance;
	return &instance;
}

uint8_t StarterUart::GetLatestChar()
{
	if (htim.Instance->SR & USART_SR_RXNE)
	{
		latestChar = htim.Instance->DR;
	}

	return latestChar;
}

void StarterUart::InitUart()
{
	__HAL_RCC_UART4_CLK_ENABLE();

	htim.Instance              = UART4;
	htim.Init.BaudRate         = 115200;
	htim.Init.WordLength       = UART_WORDLENGTH_8B;
	htim.Init.StopBits         = UART_STOPBITS_1;
	htim.Init.Parity           = UART_PARITY_NONE;
	htim.Init.Mode             = UART_MODE_RX;
	htim.Init.HwFlowCtl        = UART_HWCONTROL_NONE;
	htim.Init.OverSampling     = UART_OVERSAMPLING_16;

	HAL_UART_Init(&htim);
}

void StarterUart::InitGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/**USART4 GPIO Configuration
	PA0      ------> USART1_TX
	PA1      ------> USART1_RX
	*/
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin          = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode         = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull         = GPIO_PULLUP;
	GPIO_InitStruct.Speed        = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate    = GPIO_AF8_UART4;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
