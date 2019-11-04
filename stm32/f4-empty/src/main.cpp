#include "stm32f4xx_hal.h"

int main(void)
{
	GPIO_InitTypeDef LD2_InitStruct =
	{
		.Pin   = 1 << 5,
		.Mode  = GPIO_MODE_OUTPUT_PP,
		.Pull  = GPIO_NOPULL,
		.Speed = GPIO_SPEED_FREQ_LOW
	};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_Init(GPIOA, &LD2_InitStruct);

	while (1)
	{
		for (int i = 0; i < 200000; i++);
		HAL_GPIO_TogglePin(GPIOA, 1 << 5);
	}
}
