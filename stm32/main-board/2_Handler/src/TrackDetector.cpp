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
	front.receiver = LsUartFront::GetInstance();
	rear.receiver  = LsUartRear ::GetInstance();
}

TrackDetector* TrackDetector::GetInstance()
{
	static TrackDetector instance;
	return &instance;
}

void TrackDetector::SetMode(TrackMode mode)
{
	this->mode = mode;
}

TrackType TrackDetector::GetTrackType()
{
	return trackType;
}

float TrackDetector::GetFrontLine()
{
	return front.pos;
}

float TrackDetector::GetRearLine()
{
	return rear.pos;
}

void TrackDetector::GetChosenOne(LineData& line, LineDirection dir)
{
	int16_t lineval_mm = 0;

	switch (line.input.cnt)
	{
		case 0:
		{
			// prev = prev
			break;
		}
		case 1:
		{
			lineval_mm = line.input.lines[0];
			break;
		}
		case 2:
		{
			switch (dir)
			{
				case ld_Left:
				{
					lineval_mm = line.input.lines[0];
					break;
				}
				case ld_Middle:
				{
					lineval_mm = line.input.lines[0];
					break;
				}
				case ld_Right:
				{
					lineval_mm = line.input.lines[1];
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
					lineval_mm = line.input.lines[0];
					break;
				}
				case ld_Middle:
				{
					lineval_mm = line.input.lines[1];
					break;
				}
				case ld_Right:
				{
					lineval_mm = line.input.lines[2];
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

	line.pos = lineval_mm / 1000.0f;
}

float TrackDetector::GetFrontLine(LineDirection const dir)
{
	GetChosenOne(front, dir);

	return front.pos;
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

bool TrackDetector::GetLineData(LineData& line)
{
	uint8_t base64_buf[50];
	size_t  base64_size;
	bool ret = false;

	line.receiver->Receive(base64_buf, base64_size, 50);

	while (base64_size > 0)
	{
		if (base64_size == 11)
		{
			uint8_t decoded_buf[11];
			size_t  decoded_size;

			base64_decode(base64_buf, decoded_buf, base64_size, &decoded_size);

			if (decoded_size == sizeof(LineInput))
			{
				line.input = *((LineInput*) decoded_buf);
				ret = true;
				break;
			}
		}

		line.receiver->Receive(base64_buf, base64_size, 50);
	}

	return ret;
}

void TrackDetector::GetNearest(LineData& line)
{
	if (line.input.cnt > 0)
	{
		static int16_t prevLine = 0;

		int16_t minDiff = abs(line.input.lines[0] - prevLine);
		uint16_t minDiffIndex = 0;

		for (int i = 1; i < line.input.cnt; i++)
		{
			uint16_t diff = abs(line.input.lines[i] - prevLine);

			if (diff < minDiff)
			{
				minDiff = diff;
				minDiffIndex = i;
			}
		}

		prevLine     = line.input.lines[minDiffIndex];
		line.pos     = prevLine / 1000.0; // mm -> m
	}
}

void TrackDetector::FilterCnt(LineData& line)
{
	if (line.prevCnt == line.input.cnt)
	{
		line.sameCntSamples++;

		if (line.sameCntSamples >= LINE_CNT_FILTER_SIZE)
		{
			line.filteredCnt = line.input.cnt;
		}
	}
	else
	{
		line.prevCnt = line.input.cnt;
		line.sameCntSamples = 1;
	}
}

void TrackDetector::EvalRaceTrackType()
{
	static enum {slow, accel, fast, brake} state = slow;
	static WaitDistance wait;
	static int typeIndex;

	switch (state)
	{
		case slow:
		{
			if (front.filteredCnt == 3)
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
			if (front.filteredCnt == 1 && wait.IsExpired())
			{
				state = fast;
				trackType = TrackType::Single;
				typeIndex++;
			}
			break;
		}
		case fast:
		{
			if (front.filteredCnt == 3)
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
			if (front.filteredCnt == 1 && wait.IsExpired())
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
	while (GetLineData(front) == true)
	{
		FilterCnt(front);
		GetNearest(front);

		frontCnt++;
	}

	// Rear
	while (GetLineData(front) == true)
	{
		FilterCnt(rear);
		GetNearest(rear);

		rearCnt++;
	}

	// Track type
	EvalRaceTrackType();
}
