#include "SensorMeasure.h"
#include "ShiftReg.h"
#include "StddevEval.h"

#include "Display.h"
#include "SensorDriver.h"
#include "System.h"

#include "SensorCfg.h"
#include "SensorSettings.h"

int main(void)
{
	System::Init();

	SensorMeasure* adc = SensorMeasure::GetInstance();
	SensorDriver sensors;
	StddevEval eval;
	Display d;

	AdcMeasType measurements[SENSOR_SIZE];
	Line l;

	while (1)
	{
		for (int i = 0; i < IR_GROUP_SIZE; i++)
		{
			sensors.SetSensors(IR_GROUP_SIZE, i);
			HAL_Delay(1); // TODO us delay

			adc->Measure((AdcInput)(i+0));
			adc->Measure((AdcInput)(i+IR_GROUP_SIZE));
		}

		adc->GetMeasurements(measurements);

		eval.Feed(measurements);
		l = eval.GetLine();
		d.DisplayLinePos(l);
	}
}

// TODO UART
// TODO minden 2. vagy 3. LED világít
// TODO nem blokkoló hívások
// TODO Időmérés (usec)
// TODO ütemező
// TODO Sensor settings, cfg
