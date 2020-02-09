#pragma once

#include "MazeDetectorSM.h"
#include "StringQueue.h"

#define LINE_SAMPLING_CYCLE    5 /* ms */
#define LINE_CNT_FILTER_SIZE   3

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
	void Process(); // For the OS task

	void SetMode(TrackMode mode);
	TrackType GetTrackType();
	float GetFrontLine(); /* m */
	float GetRearLine(); /* m */

	bool IsJunction(TrackType const track);
	bool IsFork(TrackType track);
	float GetFrontLine(LineDirection const dir);

private:
	Transmitter* trace;

	TrackMode mode;

	MazeDetectorSM frontStm;
	MazeDetectorSM rearStm;

	LineData front = {0};
	LineData rear  = {0};

	TrackDetector();

	// Process:
	bool GetLineData(LineData& line);
	void FilterCnt(LineData& line);

	void EvalRaceTrackType();

	// ---------------------------

	void EvalLineType(LineData& line);
	void GetNearest(LineData& line);
	void GetChosenOne(LineData& line, LineDirection dir);
};
