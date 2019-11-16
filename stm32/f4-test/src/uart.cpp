#include "uart.h"

static Uart* uarts[UartNum]; // TODO put this somehow into the class

Uart::Uart(UartInstance instance)
{
	uarts[instance] = this;

	__HAL_RCC_USART1_CLK_ENABLE();

	GpioInit(PA9, PA10);

	SetInterrupt(5, 0);
}

void Uart::GpioInit(GpioPin txPin, GpioPin rxPin)
{
	const uint8_t AF[] = {
		[Uart1] = GPIO_AF7_USART1,
		[Uart2] = GPIO_AF7_USART2,
		[Uart3] = GPIO_AF7_USART3,
		[Uart4] = GPIO_AF8_UART4,
		[Uart5] = GPIO_AF8_UART5,  // TODO which one? (AF7/AF8)
		[Uart6] = GPIO_AF8_USART6
	};

	Gpio::Init(PA9,  GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART1); // Tx
	Gpio::Init(PA10, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART1); // Rx
}

void ClkEnable()
{

}

void Uart::SetInterrupt(uint32_t PreemptPriority, uint32_t SubPriority)
{
	static const IRQn_Type irqn[] =
	{
		[Uart1] = USART1_IRQn,
		[Uart2] = USART2_IRQn,
		[Uart3] = USART3_IRQn,
		[Uart4] = UART4_IRQn,
		[Uart5] = UART5_IRQn,
		[Uart6] = USART6_IRQn
	};

	HAL_NVIC_SetPriority(irqn[instance], PreemptPriority, SubPriority);
	HAL_NVIC_EnableIRQ(irqn[instance]);
}

void Uart::CallTxComplete(UartInstance inst)
{
	Uart* u = uarts[inst];

	if (u != nullptr)
	{
		u->TxComplete();
	}
}

void Uart::CallRxComplete(UartInstance inst)
{
	Uart* u = uarts[inst];

	if (u != nullptr)
	{
		u->RxComplete();
	}
}

UartInstance Uart::HandleToInstance(UART_HandleTypeDef* huart)
{
	UartInstance ret = Uart1; // TODO better default return value

	for (int i = 0; i < UartNum; i++)
	{
		if (huart == &uarts[i]->handle)
		{
			ret = (UartInstance) i;
			break;
		}
	}

	return ret;
}

void Uart::CallIrqHandler(UartInstance inst)
{
	Uart* u = uarts[inst];

	if (u != nullptr)
	{
		HAL_UART_IRQHandler(&u->handle);
	}
}
