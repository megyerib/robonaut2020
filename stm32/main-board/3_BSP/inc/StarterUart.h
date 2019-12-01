#pragma once

#include "Stm32Uart.h"

class StarterUart : public Stm32Uart
{
	uint8_t rxChar = 0;

	StarterUart();

	void TxCompleteCallback() override;
	void RxCompleteCallback() override;
	void Init();

public:
	static StarterUart* GetInstance();

	uint8_t GetLastChar();
};
