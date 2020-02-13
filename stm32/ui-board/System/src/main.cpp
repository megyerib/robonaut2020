#include "System.h"
#include "MatrixDisplay.h"
#include "stm32f0xx_hal.h"
#include "Buttons.h"

#define COUNTER_PERIOD 150

int main(void)
{
	System::Init();

	MatrixDisplay* matrix = MatrixDisplay::GetInstance();
	Buttons* buttons = Buttons::GetInstance();

	int num = 0;
	bool flashing = false;
	int cntr = 0;
	bool showNum = true;

	while (1)
	{
		if (flashing)
		{
			// Set number
			if (buttons->GetRisingEdge(ButtonRight) == true)
			{
				num++;
			}
			if (buttons->GetRisingEdge(ButtonLeft) == true)
			{
				num--;
			}
			if (buttons->GetRisingEdge(ButtonUp) == true)
			{
				num += 10;
			}
			if (buttons->GetRisingEdge(ButtonDown) == true)
			{
				num -= 10;
			}

			if (num < 0)
			{
				num = 0;
			}
			else if (num > 99)
			{
				num = 99;
			}
		}

		// Set flashing
		if (buttons->GetRisingEdge(ButtonA) == true)
		{
			flashing = true;
			cntr = 0;
			showNum = false;
			// TODO CLEAR EDGES
		}
		if (buttons->GetRisingEdge(ButtonB) == true)
		{
			flashing = false;
			showNum = true;
		}

		// Count flashing
		if (flashing)
		{
			cntr++;

			if (cntr >= COUNTER_PERIOD)
			{
				cntr = 0;
				showNum = !showNum;
			}
		}

		// Display
		if (showNum)
		{
			matrix->DisplayInt(num);
		}
		else
		{
			matrix->Clear();
		}

		matrix->Refresh();

		buttons->Process();

		HAL_Delay(1);
	}

	return 0;
}
