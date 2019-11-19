#include "ledtask.h"
#include "stm32f4xx_hal.h"

LedTask::LedTask() : CyclicTask((char*)"LED", 500, 1, configMINIMAL_STACK_SIZE)
{
	// FreeRTOS related stuff
}

LedTask* LedTask::Create()
{
	return new LedTask();
}

void LedTask::TaskInit()
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
}

void LedTask::TaskFunction()
{
	HAL_GPIO_TogglePin(GPIOA, 1 << 5);
}
