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
	static char rxBuf[100]; // static -> doesn't consume task
	size_t size;
	uart->Receive(rxBuf, &size);

	static char buf[] = "Hello!\n";
	uart->Send(buf, 7);
}
