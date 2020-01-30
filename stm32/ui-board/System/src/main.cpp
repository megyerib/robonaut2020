#include "System.h"
#include "ShiftReg.h"

#include "MatrixImg.h"

typedef struct
{
	uint8_t ledImg;
	uint8_t col;
	uint8_t row;
}
OUTPUT_IMAGE;


int main(void)
{
	System::Init();
	ShiftReg* sr = ShiftReg::GetInstance();

	static OUTPUT_IMAGE buf;

	uint32_t i = 0;

	while (1)
	{
		buf.row = img_big7[i];
		buf.col = ~(1 << i);

		sr->Transmit(&buf, 3);

		i++;
		i %= 8;

		HAL_Delay(1);
	}

	return 0;
}
