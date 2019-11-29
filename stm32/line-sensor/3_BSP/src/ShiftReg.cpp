#include "../inc/ShiftReg.h"

#include "../inc/Spi.h"
#include "stm32f0xx_hal.h"


ShiftReg::ShiftReg(uint8_t OE_Pin, uint8_t LE_Pin)
{
	spi = Spi::GetInstance();

	this->LE_Pin = 1 << LE_Pin;
	this->OE_Pin = 1 << OE_Pin;
	GpioInit();

	HAL_GPIO_WritePin(GPIOB, this->OE_Pin, GPIO_PIN_RESET); // High active -> Set
	HAL_GPIO_WritePin(GPIOB, this->LE_Pin, GPIO_PIN_RESET); // Low active  -> Reset
}

bool ShiftReg::Display(void* data, size_t size)
{
	bool ret = false;

	if (spi->isReady())
	{
		spi->Send(data, size);

		// TODO non-blocking!
		while(!spi->isReady());

		HAL_GPIO_WritePin(GPIOB, LE_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, LE_Pin, GPIO_PIN_RESET);

		ret = true;
	}

	return ret;
}

void ShiftReg::GpioInit()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, LE_Pin|OE_Pin, GPIO_PIN_RESET);

	/* Configure GPIO pins */
	GPIO_InitStruct.Pin = LE_Pin|OE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
