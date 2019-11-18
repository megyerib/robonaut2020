#pragma once

#include "ShiftReg.h"
#include "Line.h"


class Display
{
	ShiftReg leds;

public:
	Display();
	void DisplayLinePos(Line l);

private:
	uint8_t mmToLedPos(int16_t mm);
};
