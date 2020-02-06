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
    DeadEnd,
    CrossingPoint
}
TrackType;

typedef enum
{
    ld_NoLine,
    ld_Left,
    ld_Middle,
    ld_Right
} LineDirection;

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
	void Process(); // For the OS task

	void SetMode(TrackMode mode);
	TrackType GetTrackType();
	float GetFrontLine(); /* m */
	float GetRearLine(); /* m */

	bool IsJunction(TrackType const track);
	bool IsFork(TrackType track);
	float GetFrontLine(LineDirection const dir);

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
