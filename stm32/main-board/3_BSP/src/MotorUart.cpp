#include <MotorUart.h>
#include <cstring>
#include "NvicPrio.h"

MotorUart::MotorUart() : Stm32Uart(Uart4)
{
	Init();

	//HAL_UART_Receive_IT(&handle, &rxBuffer[rxBufSize], 1);
}

MotorUart* MotorUart::GetInstance()
{
	static MotorUart instance;

	return &instance;
}

void MotorUart::Send(uint8_t* buffer, size_t size)
{
	if (size > BUFFER_MAX_SIZE)
	{
		size = BUFFER_MAX_SIZE;
	}

	memcpy(txBuffer, buffer, size);
	txBufSize = size;

	HAL_UART_Transmit_IT(&handle, txBuffer, txBufSize);
}

void MotorUart::TxCompleteCallback()
{

}

void MotorUart::RxCompleteCallback()
{
	// HAL_UART_Receive_IT(&handle, &rxBuffer[rxBufSize], 1);
}

void MotorUart::Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// GPIO --------------------------------------------------------------------

	__HAL_RCC_GPIOA_CLK_ENABLE();

	/**UART4 GPIO Configuration
	PA0-WKUP     ------> UART4_TX
	PA1          ------> UART4_RX
	*/
	GPIO_InitStruct.Pin       = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// USART peripheral --------------------------------------------------------

	__HAL_RCC_UART4_CLK_ENABLE();

	handle.Instance          = UART4;
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

	HAL_NVIC_SetPriority(UART4_IRQn, MOTOR_UART_NVIC_PRIO, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);
}
