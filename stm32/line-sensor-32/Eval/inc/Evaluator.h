#pragma once

#include "Adc.h"
#include "Line.h"

class Evaluator
{
public:
	virtual void Feed(AdcMeasType meas[SENSOR_SIZE]) = 0;
	virtual Line GetLine() = 0;
};
