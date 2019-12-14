#include "TrackDetector.h"
#include <cstddef>
#include "base64.h"
#include <cstdlib>
#include "WaitTime.h"

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
	return type;
}

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
	static enum {single, triple} state = single;
	static WaitTime wait;
	static int typeIndex;

	switch (state)
	{
		case single:
		{
			if (frontLineCnt == 3)
			{
				wait.Wait_s(2);
				state = triple;
				typeIndex++;
			}
			break;
		}
		case triple:
		{
			if (frontLineCnt == 1 && wait.IsExpired())
			{
				state = single;
				typeIndex++;
			}
			break;
		}
	}

	// Get track type
	const TrackType SpeedrunTypes[4] =
	{
		Single,
		Acceleration,
		Single,
		Braking
	};

	typeIndex %= 4;
	type = SpeedrunTypes[typeIndex];
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
