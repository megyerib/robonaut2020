#pragma once

#include "DmaUart.h"

class LsUartFront : public DmaUart
{
public:
	static LsUartFront* GetInstance();

private:
	LsUartFront();
};
