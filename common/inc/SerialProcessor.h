#pragma once

#include "DmaUart.h"

// TODO fancy design pattern!!!

class SerialProcessor
{
public:
	SerialProcessor(DmaUart* uart, size_t rxBufSize);
	~SerialProcessor();
	void GetLastLine(void* buf, size_t* site);

private:
	DmaUart* uart;
	size_t rxBufSize;

	uint8_t* rxBuf;
	size_t rxBufIndex = 0;
};
