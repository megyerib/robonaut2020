#include "uart.h"

extern "C" void USART1_IRQHandler(void)
{
	Uart::CallIrqHandler(Uart1);
}

extern "C" void USART2_IRQHandler(void)
{
	Uart::CallIrqHandler(Uart2);
}

extern "C" void USART3_IRQHandler(void)
{
	Uart::CallIrqHandler(Uart3);
}

extern "C" void UART4_IRQHandler(void)
{
	Uart::CallIrqHandler(Uart4);
}

extern "C" void UART5_IRQHandler(void)
{
	Uart::CallIrqHandler(Uart5);
}

extern "C" void USART6_IRQHandler(void)
{
	Uart::CallIrqHandler(Uart6);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	UartInstance inst = Uart::HandleToInstance(huart);
	Uart::CallTxComplete(inst);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UartInstance inst = Uart::HandleToInstance(huart);
	Uart::CallTxComplete(inst);
}

// TODO are they needed for usarts?
/*void HAL_USART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	Uart::CallTxComplete(huart);
}

void HAL_USART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	Uart::CallRxComplete(huart);
}*/
