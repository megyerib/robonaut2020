#pragma once

#include "stm32f0xx_hal.h"

#ifndef HAL_SPI_MODULE_ENABLED
	#error Define HAL_SPI_MODULE_ENABLED in stm32xxxx_hal_conf.h
#endif

#define LOCAL_BUFFER_SIZE (4u)

class Spi1
{
	static SPI_HandleTypeDef handle; // TODO remove static
	uint8_t localBuffer[LOCAL_BUFFER_SIZE];

	Spi1();

	void MspInit();
	void Init();

public:
	static Spi1* GetInstance();
	void Send(void* data, size_t size);
	bool isReady();
};
