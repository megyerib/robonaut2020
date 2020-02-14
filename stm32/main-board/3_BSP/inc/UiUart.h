#pragma once

#include "stm32f4xx_hal.h"
#include "Receiver.h"

class UiUart : public Receiver
{
public:
	static UiUart* GetInstance();

	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

private:
	UART_HandleTypeDef huart;

	UiUart();

	void InitUart();
	static void InitGpio();
};
