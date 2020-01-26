#include "Trace.h"
#include "StringQueue.h"

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
