#include "Spi1.h"
#include <string.h>

static SPI_HandleTypeDef* hspi1;
SPI_HandleTypeDef Spi1::handle;

Spi1::Spi1()
{
	hspi1 = &handle;

	MspInit();
	Init();
}

Spi1* Spi1::GetInstance()
{
	static Spi1* instance = new Spi1();

	return instance;
}

void Spi1::MspInit()
{
	// MSP = MCU Support Package

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* SPI1 clock enable */
	__HAL_RCC_SPI1_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**SPI1 GPIO Configuration
	PA5     ------> SPI1_SCK
	PA7     ------> SPI1_MOSI
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* SPI1 interrupt Init */
	HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SPI1_IRQn);
}

void Spi1::Init()
{
	handle.Instance = SPI1;
	handle.Init.Mode = SPI_MODE_MASTER;
	handle.Init.Direction = SPI_DIRECTION_2LINES;
	handle.Init.DataSize = SPI_DATASIZE_8BIT;
	handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
	handle.Init.CLKPhase = SPI_PHASE_1EDGE;
	handle.Init.NSS = SPI_NSS_SOFT;
	handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	handle.Init.TIMode = SPI_TIMODE_DISABLE;
	handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	handle.Init.CRCPolynomial = 7;
	handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	handle.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;

	if (HAL_SPI_Init(&handle) != HAL_OK)
	{
		//Error_Handler();
	}
}

void Spi1::Send(void* data, size_t size)
{
	if (size <= LOCAL_BUFFER_SIZE)
	{
		memcpy(localBuffer, data, size);
		HAL_SPI_Transmit(&handle, localBuffer, size, HAL_MAX_DELAY);
	}
}

bool Spi1::isReady()
{
	return HAL_SPI_GetState(&handle) == HAL_SPI_STATE_READY;
}

extern "C" void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(hspi1);
}
