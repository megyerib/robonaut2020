#include "StringQueue.h"

StringQueue* StringQueue::GetInstance(STR_QUEUE_ID id)
{
	static StringQueue* instances[StringQueueNum] = {nullptr};

	if (instances[id] == nullptr)
	{
		instances[id] = new StringQueue(id, stringQueueSize[id]);
	}

	return instances[id];
}

StringQueue::StringQueue(STR_QUEUE_ID id, size_t size)
{
	handle = xMessageBufferCreate(size);
}

int32_t StringQueue::Transmit(const void* buffer, size_t size)
{
	xMessageBufferSend(handle, buffer, size, 0 /*Ticks to wait*/);

	return TRANSMIT_OK;
}

int32_t StringQueue::Receive(void* buffer, size_t& size, size_t targetSize)
{
	size = xMessageBufferReceive(handle, buffer, targetSize, 0 /*Ticks to wait*/);

	return RECEIVE_OK;
}
