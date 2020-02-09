#include "MazeDetectorSM.h"

MazeDetectorSM::MazeDetectorSM(LineData& line) : line(line)
{

}

void MazeDetectorSM::Process()
{
	(this->*stateMatrix[state][line.lType])();
}

void MazeDetectorSM::RegFork2()
{
	state = td_2Fork;

	type  = Fork2;
	dir   = ld_Middle;
}

void MazeDetectorSM::RegFork3()
{
	state = td_3Fork;

	type  = Fork3;
	dir   = ld_Middle;
}

void MazeDetectorSM::RegJct2()
{
	state = td_2Junction;
	type  = Junction2;

	switch (line.chosenIndex)
	{
	case 0:
		dir = ld_Left;
		break;
	case 1:
		dir = ld_Right;
		break;
	}
}

void MazeDetectorSM::RegJct3()
{
	state = td_3Junction;
	type  = Junction3;

	switch (line.chosenIndex)
	{
	case 0:
		dir = ld_Left;
		break;
	case 1:
		dir = ld_Middle;
		break;
	case 2:
		dir = ld_Right;
		break;
	}
}

void MazeDetectorSM::Reg1()
{
	state = td_1;

	type  = Single;
	dir   = ld_Middle;
}

void MazeDetectorSM::RegEnd()
{
	state = td_End;

	type  = DeadEnd;
	dir   = ld_Middle;
}

void MazeDetectorSM::RegExit()
{
	// TODO Internal state etc
	state = td_1;
}
