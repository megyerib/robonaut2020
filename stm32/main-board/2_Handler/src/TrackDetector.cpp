#include "TrackDetector.h"
#include <stddef.h>
#include "base64.h"

TrackDetector::TrackDetector()
{
	uartFront = LsUart3::GetInstance();
	uartRear  = LsUart5::GetInstance();
}

TrackDetector* TrackDetector::GetInstance()
{
	static TrackDetector instance;
	return &instance;
}

void TrackDetector::SetMode(TrackMode mode)
{

}

TrackType TrackDetector::GetTrackType()
{
	// TODO
	return None;
}

float TrackDetector::GetFrontLine()
{
	// TODO
	return 0;
}

float TrackDetector::GetRearLine()
{
	// TODO
	return 0;
}

void TrackDetector::GetFrontLineData()
{
	uint8_t base64_buf[30];
	size_t  base64_size;

	uint8_t decoded_buf[30];
	size_t  decoded_size;

	uartFront->GetMessage(base64_buf, &base64_size);

	if (base64_size > 0)
	{
		base64_decode(base64_buf, decoded_buf, base64_size, &decoded_size);

		if (decoded_size == sizeof(Line))
		{
			frontLine = *((Line*) decoded_buf);
		}
	}
}

void TrackDetector::GetRearLineData()
{
	uint8_t base64_buf[30];
	size_t  base64_size;

	uint8_t decoded_buf[30];
	size_t  decoded_size;

	uartRear->GetMessage(base64_buf, &base64_size);

	if (base64_size > 0)
	{
		base64_decode(base64_buf, decoded_buf, base64_size, &decoded_size);

		if (decoded_size == sizeof(Line))
		{
			rearLine = *((Line*) decoded_buf);
		}
	}
}

void TrackDetector::Process()
{
	GetFrontLineData();
	GetRearLineData();
}
