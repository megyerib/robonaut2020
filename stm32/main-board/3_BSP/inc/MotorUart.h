#pragma once

#include "Stm32Uart.h"

#define BUFFER_MAX_SIZE  30

class MotorUart : public Stm32Uart
{
	uint8_t txBuffer[BUFFER_MAX_SIZE];
	size_t txBufSize = 0;

	MotorUart();

	void TxCompleteCallback() override;
	void RxCompleteCallback() override;
	void Init();

public:
	static MotorUart* GetInstance();

	void Send(uint8_t* buffer, size_t size);
};
