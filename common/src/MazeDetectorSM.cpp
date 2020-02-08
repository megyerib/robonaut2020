#include "MazeDetectorSM.h"

MazeDetectorSM::MazeDetectorSM(LineData& line) : line(line)
{

}

void MazeDetectorSM::RegFork2()
{
	state = td_2Fork;
	type  = ForkLeft; // TODO Which one?
}

void MazeDetectorSM::RegFork3()
{
	state = td_3Fork;
	type  = ForkLeft; // TODO Which one?
}

void MazeDetectorSM::RegJct2()
{
	state = td_3Junction;
	type  = JunctionLeft;
}

void MazeDetectorSM::RegJct3()
{
	state = td_3Junction;
	type  = JunctionBoth;
}

void MazeDetectorSM::Reg1()
{
	state = td_1;
	type  = Single;
}

void MazeDetectorSM::RegEnd()
{
	state = td_End;
	type  = DeadEnd;
}

void MazeDetectorSM::RegExit()
{
	// TODO Internal state etc
}
