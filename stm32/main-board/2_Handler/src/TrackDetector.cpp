#include "TrackDetector.h"
#include <cstddef>
#include "base64.h"
#include <cstdlib>
#include "WaitDistance.h"

#define LINE_CNT_FILTER_SIZE   4

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
	return trackType;
}

// TODO thread safety!!!
float TrackDetector::GetFrontLine()
{
	return frontLinePos;
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

void TrackDetector::EvalFrontLine()
{
	// Set filtered line count
	static int linecnts[4] = {0};

	for (int i = 0; i < 4; i++)
	{
		if (i == frontLine.cnt)
		{
			linecnts[i]++;

			if (linecnts[i] >= LINE_CNT_FILTER_SIZE)
				frontLineCnt = i;
		}
		else
		{
			linecnts[i] = 0;
		}
	}

	// Get nearest line
	if (frontLine.cnt > 0)
	{
		static int16_t prevLine = 0;

		int16_t minDiff = abs(frontLine.lines[0] - prevLine);
		uint16_t minDiffIndex = 0;

		for (int i = 1; i < frontLine.cnt; i++)
		{
			uint16_t diff = abs(frontLine.lines[i] - prevLine);

			if (diff < minDiff)
			{
				minDiff = diff;
				minDiffIndex = i;
			}
		}

		prevLine     = frontLine.lines[minDiffIndex];
		frontLinePos = prevLine / 1000.0; // mm -> m
	}
}

void TrackDetector::EvalTrackType()
{
	static enum {slow, accel, fast, brake} state = slow;
	static WaitDistance wait;
	static int typeIndex;

	switch (state)
	{
		case slow:
		{
			if (frontLineCnt == 3)
			{
				wait.Wait_m(1.1);
				state = accel;
				trackType = TrackType::Acceleration;
				typeIndex++;
			}
			break;
		}
		case accel:
		{
			if (frontLineCnt == 1 && wait.IsExpired())
			{
				state = fast;
				trackType = TrackType::Single;
				typeIndex++;
			}
			break;
		}
		case fast:
		{
			if (frontLineCnt == 3)
			{
				wait.Wait_m(3.1);
				state = brake;
				trackType = TrackType::Braking;
				typeIndex++;
			}
			break;
		}
		case brake:
		{
			if (frontLineCnt == 1 && wait.IsExpired())
			{
				state = slow;
				trackType = TrackType::Single;
				typeIndex++;
			}
			break;
		}
	}
}

void TrackDetector::Process()
{
	// Front
	GetFrontLineData();
	EvalFrontLine();

	// Rear

	// Track type
	EvalTrackType();
}
