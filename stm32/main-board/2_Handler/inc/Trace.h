#pragma once

#include <stdint.h>
#include <stddef.h>
#include "TraceUart.h"

class Trace
{
public:
	static Trace* GetInstance();
	void Process();

private:
	TraceUart* uart;

	Trace();
};
