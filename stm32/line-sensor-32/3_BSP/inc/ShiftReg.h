#pragma once

#include <stddef.h>

#include "Spi.h"

class ShiftReg
{
	Spi* spi;

	uint32_t LE_Pin;
	uint32_t OE_Pin;

	void GpioInit();

public:
	ShiftReg(uint8_t OE_Pin, uint8_t LE_Pin);
	bool Display(void* data, size_t size);
};
