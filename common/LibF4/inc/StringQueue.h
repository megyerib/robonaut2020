#pragma once

#include "Transmitter.h"
#include "Receiver.h"

#include "FreeRTOS.h"
#include "message_buffer.h"

// HOW TO USE
//
// 1. Specify a queue ID in the STR_QUEUE_ID                                TestTrace
// 2. Set the queue size in characters in the stringQueueSize[] array       [TestTrace] = 100
// 3. Instantiate a StringQueue in your task with the proper ID             Transmitter* msgQueue;
//                                                                          msgQueue = StringQueue::GetInstance(TestTrace);
// 4. Talk into the transmitter                                             static char buffer1[] = "Test";
//	                                                                        msgQueue->Transmit(buffer1, 4);
// Like and subscribe

typedef enum
{
	TestTrace = 0,
	TrackDetectorTrace,
	Race,
	MapModule,
	ServoCalib,

	StringQueueNum
}
STR_QUEUE_ID;

const size_t stringQueueSize[] =
{
	[TestTrace] = 100,
	[TrackDetectorTrace] = 200,
	[Race] = 200,
	[MapModule] = 200,
	[ServoCalib] = 200
};

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
