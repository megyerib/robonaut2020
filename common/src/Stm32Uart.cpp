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

#ifdef STM32F446xx
extern "C" void USART1_IRQHandler()
{
	Stm32Uart::IrqHandler(Uart1);
}

// TODO Re-enable
/*extern "C" void USART2_IRQHandler()
{
	Stm32Uart::IrqHandler(Uart2);
}*/

extern "C" void USART3_IRQHandler()
{
	Stm32Uart::IrqHandler(Uart3);
}

extern "C" void UART4_IRQHandler()
{
	Stm32Uart::IrqHandler(Uart4);
}

extern "C" void UART5_IRQHandler()
{
	Stm32Uart::IrqHandler(Uart5);
}

extern "C" void USART6_IRQHandler()
{
	Stm32Uart::IrqHandler(Uart3);
}
#endif /* STM32F446xx */

#ifdef STM32F030xx
extern "C" void USART1_IRQHandler()
{
	Stm32Uart::IrqHandler(Uart1);
}
#endif /* STM32F030xx */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
	Stm32Uart::CallTxCompleteCallback(huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	Stm32Uart::CallRxCompleteCallback(huart);
}
