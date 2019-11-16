#include "stm32f0xx_hal.h"
#include "Rcc.h"
#include "LedDriver.h"
#include "Adc.h"
#include "Line.h"
#include "StddevEval.h"
#include "Display.h"

int main(void)
{
	HAL_Init();
	Rcc::Init();

	Adc* adc = Adc::GetInstance(Set4);

	LedDriver sensors(1, 11);

	uint32_t patterns[] =
	{
		0x11111111,
		0x22222222,
		0x44444444,
		0x88888888
	};

	AdcMeasType measurements[32];

	Line l;
	StddevEval eval;
	Display d;

	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			sensors.Display(&patterns[i], 4);
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

// TODO minden 2. vagy 3. LED világít
// TODO nem blokkoló hívások
// TODO Időmérés (usec)
// TODO Makrók a különböző típusú szenzorokhoz
// TODO ütemező
// TODO system osztály
// TODO magic konstansok kiszedése
