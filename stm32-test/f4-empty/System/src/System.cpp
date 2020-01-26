#include "system.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

System::System()
{

}

System* System::GetInstance()
{
	static System instance;

	return &instance;
}

void System::OsStart()
{

}

void System::Clock_Config()
{

}

// Systick handler
extern "C" void SysTick_Handler(void)
{

}
