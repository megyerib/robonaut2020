#pragma once

#include "Line.h"
#include "Receiver.h"

#define LINE_SAMPLING_CYCLE    5 /* ms */

typedef enum
{
	None,
	Single,
	Acceleration,
	Braking,
    Exit,
    ExitReverse,
    ForkBoth,
    ForkLeft,
    ForkRight,
    JunctionBoth,
    JunctionLeft,
    JunctionRight,
    CrossingPoint
}
TrackType;

typedef enum
{
	//Maze,
	Speedrun
}
TrackMode;

class TrackDetector
{
public:
	static TrackDetector* GetInstance();
	void SetMode(TrackMode mode);
	TrackType GetTrackType();
	float GetFrontLine(); /* m */
	float GetRearLine(); /* m */
	void Process(); // For the OS task

private:
	Receiver* frontProcessor;
	Receiver* rearProcessor;

	Line frontLine = {0};
	Line rearLine = {0};

	float frontLinePos = 0;
	uint16_t frontLineCnt = 0;

	TrackType trackType = Single;

	TrackDetector();
	void GetFrontLineData();
	void GetRearLineData();
	void EvalFrontLine();
	void EvalRearLine();
	void EvalTrackType();
};

// TODO OS task
