#pragma once

#include "Transmitter.h"
#include "Receiver.h"

#include "FreeRTOS.h"
#include "message_buffer.h"

typedef enum
{
	TestTrace = 0,
	TofFrontTrace,

	StringQueueNum
}
STR_QUEUE_ID;

const size_t stringQueueSize[] =
{
	[TestTrace] = 100,
	[TofFrontTrace] = 100
};
// TODO check this

class StringQueue : public Transmitter,
                    public Receiver
{
public:
	static StringQueue* GetInstance(STR_QUEUE_ID id);

	// Push a message into the FIFO
	virtual int32_t Transmit(const void* buffer, size_t size) override;

	// Pop a message out of the FIFO
	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) override;

private:
	MessageBufferHandle_t handle;

	StringQueue(STR_QUEUE_ID id, size_t size);
};
