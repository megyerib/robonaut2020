#pragma once

#include <TraceUart.h>
#include <stdint.h>
#include <stddef.h>

class Trace
{
public:
	static Trace* GetInstance();
	void Process();

private:
	TraceUart* uart;

	Trace();
};
