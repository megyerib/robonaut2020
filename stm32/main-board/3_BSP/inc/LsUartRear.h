#pragma once

#include "DmaUart.h"

class LsUartRear : public DmaUart
{
public:
	static LsUartRear* GetInstance();

private:
	LsUartRear();
};
