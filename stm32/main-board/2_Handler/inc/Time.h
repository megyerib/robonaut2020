#pragma once

#include <stdint.h>

class Time
{
public:
	static Time* GetInstance();
	uint32_t GetTime(); /* us */
};
