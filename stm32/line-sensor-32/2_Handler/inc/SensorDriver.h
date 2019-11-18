#pragma once

#include "ShiftReg.h"

class SensorDriver
{
	ShiftReg sensors;

public:
	SensorDriver();
	void SetSensors(uint8_t groupSize, uint8_t index);
};
