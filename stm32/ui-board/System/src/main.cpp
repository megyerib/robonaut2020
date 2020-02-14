#include "System.h"
#include "MatrixDisplay.h"
#include "stm32f0xx_hal.h"
#include "Buttons.h"
#include "Uart.h"

#define COUNTER_PERIOD 150

int main(void)
{
	System::Init();

	MatrixDisplay* matrix = MatrixDisplay::GetInstance();
	Buttons* buttons = Buttons::GetInstance();
	Uart* uart = Uart::GetInstance();

	int num = 0;
	bool flashing = false;
	int cntr = 0;
	bool showNum = true;
	uint8_t buf;

	while (1)
	{
		bool buttonPushed = false;

		if (flashing)
		{
			// Set number
			if (buttons->GetRisingEdge(ButtonRight) == true)
			{
				num++;
				buttonPushed = true;
			}
			if (buttons->GetRisingEdge(ButtonLeft) == true)
			{
				num--;
				buttonPushed = true;
			}
			if (buttons->GetRisingEdge(ButtonUp) == true)
			{
				num += 10;
				buttonPushed = true;
			}
			if (buttons->GetRisingEdge(ButtonDown) == true)
			{
				num -= 10;
				buttonPushed = true;
			}

			if (buttonPushed)
			{
				if (num < 0)
				{
					num = 0;
				}
				else if (num > 99)
				{
					num = 99;
				}
			}
		}

		// Set flashing
		if (buttons->GetRisingEdge(ButtonA) == true)
		{
			flashing = true;
			cntr = 0;
			showNum = false;
			buttons->ClearRisingEdges();

			buf = (uint8_t)255;
			uart->Send(&buf, 1);
		}

		// Set mode
		if (buttons->GetRisingEdge(ButtonB) == true)
		{
			flashing = false;
			showNum = true;

			buf = (uint8_t)num;
			uart->Send(&buf, 1);
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
