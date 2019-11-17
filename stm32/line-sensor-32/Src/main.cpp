#include "Rcc.h"
#include "LedDriver.h"
#include "Adc.h"
#include "StddevEval.h"
#include "Display.h"
#include "SensorDriver.h"

int main(void)
{
	HAL_Init();
	Rcc::Init();

	Adc* adc = Adc::GetInstance(Set4);
	SensorDriver sensors;
	StddevEval eval;
	Display d;

	AdcMeasType measurements[32];
	Line l;

	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			sensors.SetSensors(4, i);
			HAL_Delay(1);
			adc->Measure((AdcInput) i);
			adc->Measure((AdcInput)(i+4));
		}

		adc->GetMeasurements(measurements);

		eval.Feed(measurements);
		l = eval.GetLine();
		d.DisplayLinePos(l);
	}
}

// Systick handler TODO replace
extern "C" void SysTick_Handler(void)
{
	// HAL
	HAL_IncTick();

	// FreeRTOS
	/*if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		xPortSysTickHandler();
	}*/
}

// TODO UART
// TODO minden 2. vagy 3. LED világít
// TODO nem blokkoló hívások
// TODO Időmérés (usec)
// TODO Makrók a különböző típusú szenzorokhoz
// TODO ütemező
// TODO system osztály
// TODO magic konstansok kiszedése
