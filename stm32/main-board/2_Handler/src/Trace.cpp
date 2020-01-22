#include "Trace.h"

Trace::Trace()
{
	uart = TraceUart::GetInstance();
}

Trace* Trace::GetInstance()
{
	static Trace instance;
	return &instance;
}

void Trace::Process()
{
	static char rxBuf[100] = "Hello!\n"; // static -> doesn't consume task
	static size_t bufSize = 7;

	size_t tmpSize = uart->Receive(rxBuf, 100);

	if (tmpSize > 0)
	{
		bufSize = tmpSize;
	}

	uart->Transmit(rxBuf, bufSize);
}
