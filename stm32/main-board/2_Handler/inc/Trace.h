#pragma once

#include <stdint.h>
#include <stddef.h>

class Trace
{
public:
	static Trace* GetInstance();
	void TraceFrame(uint8_t* buf, size_t size);
	void TraceString(uint8_t* string);
};
