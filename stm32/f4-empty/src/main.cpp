#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

void LedTask(void* p)
{
	UNUSED(p);

	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, 1 << 5);
		vTaskDelay(2000);
	}
}

void IdleTask(void* p)
{
	UNUSED(p);

	while(1);
}

void OsInit()
{
	HAL_InitTick(TICK_INT_PRIORITY);

	TaskHandle_t htask1, htask2;

	// TODO valami memóriát kéne foglalni az OS-nek

	xTaskCreate (IdleTask, "IDLE", 128, NULL, tskIDLE_PRIORITY,     &htask1);
	xTaskCreate (LedTask,  "LED",  128, NULL, tskIDLE_PRIORITY + 1, &htask2);

	vTaskStartScheduler();
}

void LedInit()
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

int main(void)
{
	LedInit();
	OsInit();

	while (1);
}
