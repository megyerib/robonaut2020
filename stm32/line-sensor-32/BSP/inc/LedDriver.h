#pragma once

#include <stddef.h>
#include "Spi1.h"

class LedDriver
{
	Spi1* spi;

	uint32_t LE_Pin;
	uint32_t OE_Pin;

	void GpioInit();

public:
	LedDriver(uint8_t OE_Pin, uint8_t LE_Pin);
	bool Display(void* data, size_t size);
};
