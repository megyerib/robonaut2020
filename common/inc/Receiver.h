#pragma once

#include <cstdint>
#include <cstddef>

#define RECEIVE_OK                        ( 0)
#define RECEIVE_BUFFER_NULL               (-1)
#define RECEIVE_TARGET_SIZE_INSUFFICIENT  (-2)
#define RECEIVE_INTERNAL_ERROR            (-3)

class Receiver
{
public:
	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) = 0;
};
