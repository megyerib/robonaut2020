#include <Stm32Uart.h>

Stm32Uart* Stm32Uart::instances[UartMaxNum] = {nullptr};
uint32_t Stm32Uart::instanceNum = 0;

Stm32Uart::Stm32Uart(UartInstance inst)
{
	Stm32Uart** uart = &instances[inst];

	if (uart != nullptr)
	{
		*uart = this;
	}
}

void Stm32Uart::CallTxCompleteCallback(UART_HandleTypeDef* huart)
{
	UartInstance uart;

	if (HandleToInstance(huart, &uart) == true)
	{
		instances[uart]->TxCompleteCallback();
	}
}

void Stm32Uart::CallRxCompleteCallback(UART_HandleTypeDef* huart)
{
	UartInstance uart;

	if (HandleToInstance(huart, &uart) == true)
	{
		instances[uart]->RxCompleteCallback();
	}
}

bool Stm32Uart::HandleToInstance(UART_HandleTypeDef* huart, UartInstance* result)
{
	bool found = false;

	if (huart != nullptr)
	{
		for (int i = 0; i < UartMaxNum; i++)
		{
			Stm32Uart* uart = instances[i];

			if (uart != nullptr && &uart->handle == huart)
			{
				*result = (UartInstance) i;
				found = true;
				break;
			}
		}
	}

	return found;
}

void Stm32Uart::IrqHandler(UartInstance inst)
{
	Stm32Uart* uart = instances[inst];

	if (uart != nullptr)
	{
		HAL_UART_IRQHandler(&uart->handle);
	}
}

// Must be defined for each MCU
extern "C" void USART1_IRQHandler()
{
	Stm32Uart::IrqHandler(Uart1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
	Stm32Uart::CallTxCompleteCallback(huart);
}

/*
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
USART6_IRQHandler
*/
