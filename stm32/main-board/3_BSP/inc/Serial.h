#pragma once

#include <stddef.h>

class Serial
{
public:
	virtual size_t Transmit(void*, size_t) = 0;
	virtual size_t Receive(void*, size_t) = 0;
};
