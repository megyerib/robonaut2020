#pragma once

#include <TraceUart.h>
#include <stdint.h>
#include <stddef.h>
#include "Transmitter.h"

class Trace
{
public:
	static Trace* GetInstance();
	void Process();

	static void Print(Transmitter* tx, const char* string);
	static void Print(Transmitter* tx, const char* format, uint32_t p1);
	static void Print(Transmitter* tx, const char* format, uint32_t p1, uint32_t p2);
	static void Print(Transmitter* tx, const char* format, uint32_t p1, uint32_t p2, uint32_t p3);

private:
	TraceUart* uart;

	Trace();
};
