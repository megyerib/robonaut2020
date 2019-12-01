#pragma once

#include "LsUart3.h"
#include "LsUart5.h"
#include "Line.h"

#define LINE_SAMPLING_CYCLE    5 /* ms */

typedef enum
{
	None,
	Single,
	Acceleration,
	Braking
}
TrackType;

typedef enum
{
	Maze,
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
	void Cycle(); // For the OS task

private:
	LsUart3* uartFront;
	LsUart5* uartRear;

	Line frontLine = {0};
	Line rearLine = {0};

	TrackDetector();
	void GetFrontLineData();
	void GetRearLineData();
};

// TODO OS task
