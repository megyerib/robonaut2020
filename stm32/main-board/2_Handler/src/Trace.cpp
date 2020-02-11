#include "Trace.h"
#include "StringQueue.h"
#include <cstring>
#include <cstdio>

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
	static char rxBuf[1000];

	size_t bufSize = 0;

	for (int i = 0; i < StringQueueNum; i++)
	{
		Receiver* queue = StringQueue::GetInstance((STR_QUEUE_ID)i);

		if (queue != nullptr)
		{
			size_t size;
			queue->Receive(&rxBuf[bufSize], size, 1000 /*TODO*/);

			while (size > 0)
			{
				bufSize += size;
				rxBuf[bufSize] = '\n';
				bufSize++;

				queue->Receive(&rxBuf[bufSize], size, 1000 /*TODO*/);
			}
		}
	}

	uart->Transmit(rxBuf, bufSize);
}

void Trace::Print(Transmitter* tx, const char* string)
{
	uint32_t len = strlen(string);

	tx->Transmit(string, len);
}

void Trace::Print(Transmitter* tx, const char* format, uint32_t p1)
{
	uint32_t maxlen = strlen(format) + 10;
	char buf[maxlen];
	uint32_t len = snprintf(buf, maxlen, format, p1);
	tx->Transmit(buf, len);
}

void Trace::Print(Transmitter* tx, const char* format, uint32_t p1, uint32_t p2)
{
	uint32_t maxlen = strlen(format) + 20;
	char buf[maxlen];
	uint32_t len = snprintf(buf, maxlen, format, p1, p2);
	tx->Transmit(buf, len);
}

void Trace::Print(Transmitter* tx, const char* format, uint32_t p1, uint32_t p2, uint32_t p3)
{
	uint32_t maxlen = strlen(format) + 30;
	char buf[maxlen];
	uint32_t len = snprintf(buf, maxlen, format, p1, p2, p3);
	tx->Transmit(buf, len);
}
