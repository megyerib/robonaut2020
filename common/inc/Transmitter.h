#pragma once

#include <cstdint>
#include <cstddef>

#define TRANSMIT_OK                        ( 0)
#define TRANSMIT_BUFFER_NULL               (-1)
#define TRANSMIT_TARGET_SIZE_INSUFFICIENT  (-2)
#define TRANSMIT_INTERNAL_ERROR            (-3)
#define TRANSMIT_BUSY                      (-4)

class Transmitter
{
public:
	virtual int32_t Transmit(const void* buffer, size_t size) = 0;
};
