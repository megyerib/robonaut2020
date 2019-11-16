#include "gpio.h"
//#include "stm32f4xx_hal.h"

void Gpio::EnablePortOfPin(GpioPin pin)
{
	uint32_t port = (pin >> 4) & 0xF;

	switch (port)
	{
	case 0:
		__HAL_RCC_GPIOA_CLK_ENABLE();
		break;
	case 1:
		__HAL_RCC_GPIOB_CLK_ENABLE();
		break;
	case 2:
		__HAL_RCC_GPIOC_CLK_ENABLE();
		break;
	case 8:
		__HAL_RCC_GPIOH_CLK_ENABLE();
		break;
	default:
		break;
	}
}

Gpio* Gpio::Init(GpioPin pin, uint32_t Mode, uint32_t Pull, uint32_t Speed, uint32_t Alternate)
{
	const GPIO_TypeDef* ports[] =
	{
		GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH
	};

	Gpio* ret = new Gpio(); // In case when new features are implemented
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	ret->EnablePortOfPin(pin);

	GPIO_InitStruct.Pin = 1 << (pin & 0xF);
	GPIO_InitStruct.Mode = Mode;
	GPIO_InitStruct.Pull = Pull;
	GPIO_InitStruct.Speed = Speed;
	GPIO_InitStruct.Alternate = Alternate;

	HAL_GPIO_Init((GPIO_TypeDef*) ports[pin >> 4 & 0xF], &GPIO_InitStruct);

	return ret;
}
