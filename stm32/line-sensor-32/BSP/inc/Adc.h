#pragma once
#include "stm32f0xx_hal.h"
#include <stdint.h>

#ifndef HAL_ADC_MODULE_ENABLED
	#error Define HAL_ADC_MODULE_ENABLED in stm32xxxx_hal_conf.h
#endif

typedef enum
{
	Input0,
	Input1,
	Input2,
	Input3,
	Input4,
	Input5,
	Input6,
	Input7
}
AdcInput;

typedef enum
{
	Set3 = 3,
	Set4 = 4
}
SensorSets;

typedef uint32_t AdcMeasType;

class Adc
{
public:
	static Adc* GetInstance(SensorSets sets);
	void Measure(AdcInput input);
	void GetMeasurements(AdcMeasType* dest);

private:
	static ADC_HandleTypeDef handle;
	AdcMeasType measurements[32];
	SensorSets sets;

	Adc(SensorSets sets);

	void InitMux();
	void InitAdc();
	void InitAdcGpio();

	void SetMux(AdcInput input);
};
