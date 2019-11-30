#include <ExtUart3.h>
#include <cstring>

ExtUart3::ExtUart3() : Stm32Uart(Uart3)
{
	Init();

	HAL_UART_Receive_IT(&handle, &rxBuffer[rxBufSize], 1);
}

ExtUart3* ExtUart3::GetInstance()
{
	static ExtUart3 instance;

	return &instance;
}

void ExtUart3::Send(uint8_t* buffer, size_t size)
{
	if (size > BUFFER_MAX_SIZE)
	{
		size = BUFFER_MAX_SIZE;
	}

	memcpy(txBuffer, buffer, size);
	txBufSize = size;

	HAL_UART_Transmit_IT(&handle, txBuffer, txBufSize);
}

void ExtUart3::TxCompleteCallback()
{

}

void ExtUart3::RxCompleteCallback()
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

void ExtUart3::GetMessage(uint8_t* dst, size_t* len)
{
	__disable_irq();

	memcpy(dst, rxCpltBuffer, rxCpltBufSize);
	*len = rxCpltBufSize;
	rxCpltBufSize = 0;

	__enable_irq();
}

void ExtUart3::Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	// GPIO --------------------------------------------------------------------

	__HAL_RCC_GPIOC_CLK_ENABLE();
	/**USART3 GPIO Configuration
	PC10     ------> USART3_TX
	PC11     ------> USART3_RX
	*/
	GPIO_InitStruct.Pin       = GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// USART peripheral --------------------------------------------------------

	__HAL_RCC_USART3_CLK_ENABLE();

	handle.Instance          = USART3;
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

	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}
