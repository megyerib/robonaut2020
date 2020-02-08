#pragma once

#include "Line.h"
#include "Receiver.h"

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
	lt_1 = 0,
	lt_2Near,
	lt_2Far,
	lt_3Near,
	lt_3Far,
	lt_End,

	lt_Num
}
LineType;

typedef enum
{
	td_1 = 0,
	td_2Near,
	td_2Far,
	td_3Near,
	td_3Far,
	td_2Junction,
	td_3Junction,
	td_2Fork,
	td_3Fork,
	td_End,

	td_Num
}
TrackDetectorState;

typedef struct
{
	Receiver*           receiver;        // Line sensor receiver

	LineInput           input;           // Structure from the line receiver
	float               pos;             // Latest position of the detected line

	uint16_t            filteredCnt;     //
	uint16_t            prevCnt;         // Count of the previous line input
	uint16_t            sameCntSamples;  //

	LineType            type;            // 1, 2Near, 2Far, ...
	TrackDetectorState  mazeState;       // State of the road signal detecting mazeState machine ()
	TrackType           ret;             // This value will be returned
}
LineData;
