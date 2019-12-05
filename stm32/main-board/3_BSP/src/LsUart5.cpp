#include <LsUart5.h>
#include <cstring>
#include "NvicPrio.h"

LsUart5::LsUart5() : Stm32Uart(Uart5)
{
	Init();

	HAL_UART_Receive_IT(&handle, &rxBuffer[rxBufSize], 1);
}

LsUart5* LsUart5::GetInstance()
{
	static LsUart5 instance;

	return &instance;
}

void LsUart5::Send(uint8_t* buffer, size_t size)
{
	if (size > BUFFER_MAX_SIZE)
	{
		size = BUFFER_MAX_SIZE;
	}

	memcpy(txBuffer, buffer, size);
	txBufSize = size;

	HAL_UART_Transmit_IT(&handle, txBuffer, txBufSize);
}

void LsUart5::TxCompleteCallback()
{

}

void LsUart5::RxCompleteCallback()
{
	if (rxBuffer[rxBufSize] != '\n')
	{
		rxBufSize++;
	}
	else
	{
		__disable_irq();

		memcpy(rxCpltBuffer, rxBuffer, rxBufSize);
		rxCpltBufSize = rxBufSize;

		__enable_irq();

		rxBufSize = 0;
	}

	HAL_UART_Receive_IT(&handle, &rxBuffer[rxBufSize], 1);
}

void LsUart5::GetMessage(uint8_t* dst, size_t* len)
{
	__disable_irq();

	memcpy(dst, rxCpltBuffer, rxCpltBufSize);
	*len = rxCpltBufSize;
	rxCpltBufSize = 0;

	__enable_irq();
}

void LsUart5::Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// GPIO --------------------------------------------------------------------

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	/**UART5 GPIO Configuration
	PC12     ------> UART5_TX
	PD2      ------> UART5_RX
	*/
	GPIO_InitStruct.Pin       = GPIO_PIN_12;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin       = GPIO_PIN_2;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	// USART peripheral --------------------------------------------------------

	__HAL_RCC_UART5_CLK_ENABLE();

	handle.Instance = UART5;
	handle.Init.BaudRate = 115200;
	handle.Init.WordLength = UART_WORDLENGTH_8B;
	handle.Init.StopBits = UART_STOPBITS_1;
	handle.Init.Parity = UART_PARITY_NONE;
	handle.Init.Mode = UART_MODE_TX_RX;
	handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	handle.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&handle) != HAL_OK)
	{
		//Error_Handler();
	}

	// Interrupt enable --------------------------------------------------------

	HAL_NVIC_SetPriority(UART5_IRQn, UART5_NVIC_PRIO, 0);
	HAL_NVIC_EnableIRQ(UART5_IRQn);
}
