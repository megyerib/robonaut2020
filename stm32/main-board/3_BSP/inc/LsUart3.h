#pragma once

#include "Stm32Uart.h"

#define BUFFER_MAX_SIZE  30

class LsUart3 : public Stm32Uart
{
	uint8_t txBuffer[BUFFER_MAX_SIZE];
	size_t txBufSize = 0;

	uint8_t rxBuffer[BUFFER_MAX_SIZE];
	size_t  rxBufSize = 0;

	uint8_t rxCpltBuffer[BUFFER_MAX_SIZE];
	size_t  rxCpltBufSize = 0;

	LsUart3();

	void TxCompleteCallback() override;
	void RxCompleteCallback() override;
	void Init();

public:
	static LsUart3* GetInstance();

	void GetMessage(uint8_t* dst, size_t* len);

	void Send(uint8_t* buffer, size_t size);
};
