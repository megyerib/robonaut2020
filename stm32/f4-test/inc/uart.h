#pragma once

#include "stm32f4xx_hal.h"
#include "gpio.h"

typedef enum
{
	Uart1 = 0,
	Uart2,
	Uart3,
	Uart4,
	Uart5,
	Uart6,

	UartNum
}
UartInstance;



class Uart
{
protected:
	UartInstance instance;
	UART_HandleTypeDef handle;

public:
	Uart(UartInstance instance);

	static void CallTxComplete(UartInstance inst);
	static void CallRxComplete(UartInstance inst);
	static void CallIrqHandler(UartInstance uart);

	static UartInstance HandleToInstance(UART_HandleTypeDef* huart);

private:
	void GpioInit(GpioPin txPin, GpioPin rxPin);
	void ClkEnable();
	void SetInterrupt(uint32_t PreemptPriority, uint32_t SubPriority);

	virtual void TxComplete() = 0;
	virtual void RxComplete() = 0;
};
