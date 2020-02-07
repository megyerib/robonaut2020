#include "TrackDetector.h"
#include <cstddef>
#include "base64.h"
#include <cstdlib>
#include "WaitDistance.h"

#include "LsUartFront.h"
#include "LsUartRear.h"
#include "LastLineReader.h"
#include "LineGetterUart.h"

TrackDetector::TrackDetector()
{
	frontReceiver = LsUartFront::GetInstance();
	rearReceiver  = LsUartRear ::GetInstance();
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

float TrackDetector::GetFrontLine()
{
	return frontLinePos;
}

float TrackDetector::GetRearLine()
{
	return rearLinePos;
}

void TrackDetector::GetChosenOne(Line& line, LineDirection dir, float& prev)
{
	int16_t lineval_mm = 0;

	switch (line.cnt)
	{
		case 0:
		{
			// prev = prev
			break;
		}
		case 1:
		{
			lineval_mm = line.lines[0];
			break;
		}
		case 2:
		{
			switch (dir)
			{
				case ld_Left:
				{
					lineval_mm = line.lines[0];
					break;
				}
				case ld_Middle:
				{
					lineval_mm = line.lines[0];
					break;
				}
				case ld_Right:
				{
					lineval_mm = line.lines[1];
					break;
				}
				default:
					break;
			}

			break;
		}
		case 3:
		{
			switch (dir)
			{
				case ld_Left:
				{
					lineval_mm = line.lines[0];
					break;
				}
				case ld_Middle:
				{
					lineval_mm = line.lines[1];
					break;
				}
				case ld_Right:
				{
					lineval_mm = line.lines[2];
					break;
				}
				default:
					break;
			}

			break;
		}
		default:
			break;
	}

	prev = lineval_mm / 1000.0f;
}

float TrackDetector::GetFrontLine(LineDirection const dir)
{
	GetChosenOne(frontLine, dir, frontLinePos);

	return frontLinePos;
}

bool TrackDetector::IsJunction(TrackType track)
{
    bool track_is_junction = false;

    if ((track == TrackType::JunctionBoth) || (track == TrackType::JunctionLeft) || (track == TrackType::JunctionRight))
    {
        track_is_junction = true;
    }

    return track_is_junction;
}

bool TrackDetector::IsFork(TrackType track)
{
    bool track_is_fork = false;

    if ((track == TrackType::ForkBoth) || (track == TrackType::ForkLeft) || (track == TrackType::ForkRight))
    {
        track_is_fork = true;
    }

    return track_is_fork;
}

bool TrackDetector::GetLineData(Receiver& receiver, Line& line)
{
	uint8_t base64_buf[50];
	size_t  base64_size;
	bool ret = false;

	receiver.Receive(base64_buf, base64_size, 50);

	while (base64_size > 0)
	{
		if (base64_size == 11)
		{
			uint8_t decoded_buf[11];
			size_t  decoded_size;

			base64_decode(base64_buf, decoded_buf, base64_size, &decoded_size);

			if (decoded_size == sizeof(Line))
			{
				line = *((Line*) decoded_buf);
				ret = true;
				break;
			}
		}

		receiver.Receive(base64_buf, base64_size, 50);
	}

	return ret;
}

void TrackDetector::EvalFrontLine()
{
	GetNearest(frontLine, frontLinePos);

	frontLineCnt = frontLine.cnt; // TODO filter
}

void TrackDetector::EvalRearLine()
{
	GetNearest(rearLine, rearLinePos);

	rearLineCnt = rearLine.cnt; // TODO filter
}

void TrackDetector::GetNearest(Line& line, float& prev)
{
	if (line.cnt > 0)
	{
		static int16_t prevLine = 0;

		int16_t minDiff = abs(line.lines[0] - prevLine);
		uint16_t minDiffIndex = 0;

		for (int i = 1; i < line.cnt; i++)
		{
			uint16_t diff = abs(line.lines[i] - prevLine);

			if (diff < minDiff)
			{
				minDiff = diff;
				minDiffIndex = i;
			}
		}

		prevLine     = line.lines[minDiffIndex];
		prev = prevLine / 1000.0; // mm -> m
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
	uint16_t frontCnt = 0;
	uint16_t rearCnt  = 0;

	// Front
	while (GetLineData(*frontReceiver, frontLine) == true)
	{
		EvalFrontLine();
		frontCnt++;
	}

	// Rear
	while (GetLineData(*rearReceiver, rearLine) == true)
	{
		EvalRearLine();
		rearCnt++;
	}

	// Track type
	EvalTrackType();
}
