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
	uint8_t txBuf;
	uint8_t rxBuf;

	while (1)
	{
		bool buttonPushed = false;

		size_t size;
		uart->ReceiveByte(&rxBuf, &size);
		if (size > 0 && !flashing)
		{
			num = rxBuf;
		}

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

		// Back button
		if (buttons->GetRisingEdge(ButtonA) == true)
		{
			flashing = true;
			cntr = 0;
			showNum = false;
			buttons->ClearRisingEdges();

			txBuf = (uint8_t)255;
			uart->Send(&txBuf, 1);
		}

		// Enter button
		if (buttons->GetRisingEdge(ButtonB) == true)
		{
			flashing = false;
			showNum = true;

			// Clear Rx buffer
			size_t size;
			uart->ReceiveByte(&rxBuf, &size);

			txBuf = (uint8_t)num;
			uart->Send(&txBuf, 1);
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
