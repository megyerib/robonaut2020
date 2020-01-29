#include "System.h"
#include "ShiftReg.h"

typedef struct
{
	uint8_t ledImg;
	uint8_t col;
	uint8_t row;
}
OUTPUT_IMAGE;

typedef const uint8_t MATRIX_IMG[8];

MATRIX_IMG img_X = {0x3C, 0x7E, 0x67, 0x03, 0x03, 0x67, 0x7E, 0x3C};


int main(void)
{
	System::Init();
	ShiftReg* sr = ShiftReg::GetInstance();

	static OUTPUT_IMAGE buf;

	uint32_t i = 0;

	while (1)
	{
		buf.col = ~img_X[i];
		buf.row = 1 << i;

		sr->Transmit(&buf, 3);

		i++;
		i %= 8;

		HAL_Delay(1);
	}

	return 0;
}
