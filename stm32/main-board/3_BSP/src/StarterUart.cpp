#include <StarterUart.h>
#include <cstring>
#include "NvicPrio.h"

StarterUart::StarterUart() : Stm32Uart(Uart1)
{
	Init();

	HAL_UART_Receive_IT(&handle, &rxChar, 1);
}

StarterUart* StarterUart::GetInstance()
{
	static StarterUart instance;

	return &instance;
}

void StarterUart::TxCompleteCallback()
{

}

void StarterUart::RxCompleteCallback()
{
	HAL_UART_Receive_IT(&handle, &rxChar, 1);
}

uint8_t StarterUart::GetLastChar()
{
	return rxChar;
}

void StarterUart::Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// GPIO --------------------------------------------------------------------

	__HAL_RCC_GPIOA_CLK_ENABLE();

	/**USART1 GPIO Configuration
	PA9      ------> USART1_TX
	PA10     ------> USART1_RX
	*/
	GPIO_InitStruct.Pin       = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// USART peripheral --------------------------------------------------------

	__HAL_RCC_USART1_CLK_ENABLE();

	handle.Instance          = USART1;
	handle.Init.BaudRate     = 115200;
	handle.Init.WordLength   = UART_WORDLENGTH_8B;
	handle.Init.StopBits     = UART_STOPBITS_1;
	handle.Init.Parity       = UART_PARITY_NONE;
	handle.Init.Mode         = UART_MODE_TX_RX;
	handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	handle.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&handle) != HAL_OK)
	{
		//Error_Handler();
	}

	// Interrupt enable --------------------------------------------------------

	HAL_NVIC_SetPriority(USART1_IRQn, STARTER_UART_NVIC_PRIO, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void StarterUart::SendByte(uint8_t byte)
{
    HAL_UART_Transmit_IT(&handle, &byte, 1);
}

void StarterUart::SendWord(uint16_t word)
{
    TypeConverter conv;
    //conv.word = word;

   // HAL_UART_Transmit_IT(&handle, &conv., 2);
}

void StarterUart::SendDWord(uint32_t dword)
{


   // HAL_UART_Transmit_IT(&handle, &dword, 4);
}

void StarterUart::SendFloat(float value)
{

}
