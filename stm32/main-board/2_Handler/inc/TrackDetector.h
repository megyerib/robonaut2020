#pragma once

#include "MazeDetectorSM.h"

#define LINE_SAMPLING_CYCLE    5 /* ms */
#define LINE_CNT_FILTER_SIZE   3

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
	TrackMode mode;

	LineData front = {0};
	LineData rear  = {0};

	TrackType trackType = Single;

	TrackDetector();

	// Process:
	bool GetLineData(LineData& line);
	void FilterCnt(LineData& line);

	void EvalRaceTrackType();

	// ---------------------------

	void GetNearest(LineData& line);
	void GetChosenOne(LineData& line, LineDirection dir);
};
