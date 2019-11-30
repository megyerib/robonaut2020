#pragma once

#include "ShiftReg.h"
#include "Line.h"


class Display
{
	ShiftReg leds;

public:
	Display();
	// Line structure -> Lighting LEDs on the sensor
	void DisplayLinePos(Line l);
	void Clear();

private:
	uint8_t mmToLedPos(int16_t mm);
};
