#include "../inc/SensorMeasure.h"

#include "stm32f0xx_hal.h"
#include <string.h>

#define S0_Pin (1 << 14)
#define S1_Pin (1 << 13)
#define S2_Pin (1 << 12)
#define E_Pin  (1 <<  0) /* Low active */

ADC_HandleTypeDef SensorMeasure::handle;

SensorMeasure::SensorMeasure()
{
	InitAdcGpio();
	InitAdc();
	InitMux();
}

SensorMeasure* SensorMeasure::GetInstance()
{
	static SensorMeasure instance;

	return &instance;
}

void SensorMeasure::Measure(AdcInput input)
{
	SetMux(input);

	HAL_ADC_Start(&handle);

	for (int i = 0; i < (SENSOR_SIZE / 8); i++)
	{
		HAL_ADC_PollForConversion(&handle, HAL_MAX_DELAY);
		AdcMeasType meas = HAL_ADC_GetValue(&handle);

		measurements[31-(i*8+input)] = meas; // TODO constants
	}

	HAL_ADC_Stop(&handle);
}

void SensorMeasure::GetMeasurements(AdcMeasType* dest)
{
	size_t start = 0; // TODO multisize
	size_t size  = SENSOR_SIZE*sizeof(AdcMeasType);

	memcpy(dest, &measurements[start], size);
}

void SensorMeasure::InitMux()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, S2_Pin|S1_Pin|S0_Pin|E_Pin, GPIO_PIN_RESET); // 0 set, MUX enabled

	/* Configure GPIO pins */
	GPIO_InitStruct.Pin   = S2_Pin|S1_Pin|S0_Pin|E_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SensorMeasure::InitAdcGpio()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/**ADC GPIO Configuration
   (PA0     ------> ADC_IN0) - disabled for small sensor
	PA1     ------> ADC_IN1
	PA2     ------> ADC_IN2
	PA3     ------> ADC_IN3
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
	if (SENSOR_SIZE == 32)
	{
		GPIO_InitStruct.Pin |= GPIO_PIN_0;
	}
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SensorMeasure::InitAdc()
{
	/* ADC1 clock enable */
	__HAL_RCC_ADC1_CLK_ENABLE();

	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	*/
	handle.Instance                   = ADC1;
	handle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
	handle.Init.Resolution            = ADC_RESOLUTION_12B;
	handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	handle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
	handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
	handle.Init.LowPowerAutoWait      = DISABLE;
	handle.Init.LowPowerAutoPowerOff  = DISABLE;
	handle.Init.ContinuousConvMode    = ENABLE;
	handle.Init.DiscontinuousConvMode = DISABLE;
	handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
	handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
	handle.Init.DMAContinuousRequests = DISABLE;
	handle.Init.Overrun               = ADC_OVR_DATA_PRESERVED;

	HAL_ADC_Init(&handle);

	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	// ADC_SAMPLETIME_41CYCLES_5 is the smallest ok value

	if (SENSOR_SIZE == 32)
	{
		sConfig.Channel = ADC_CHANNEL_0;
		HAL_ADC_ConfigChannel(&handle, &sConfig);
	}

	sConfig.Channel = ADC_CHANNEL_1;
	HAL_ADC_ConfigChannel(&handle, &sConfig);

	sConfig.Channel = ADC_CHANNEL_2;
	HAL_ADC_ConfigChannel(&handle, &sConfig);

	sConfig.Channel = ADC_CHANNEL_3;
	HAL_ADC_ConfigChannel(&handle, &sConfig);

	// TODO are they needed?
	/*sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	HAL_ADC_ConfigChannel(&handle, &sConfig);

	sConfig.Channel = ADC_CHANNEL_VREFINT;
	HAL_ADC_ConfigChannel(&handle, &sConfig);*/
}

void SensorMeasure::SetMux(AdcInput input)
{
	// Propagation delay < 60 ns
	// T_clk ~ 20 ns; a function call lasts longer than that

	HAL_GPIO_WritePin(GPIOB, S2_Pin, GPIO_PinState((input >> 2) & 1));
	HAL_GPIO_WritePin(GPIOB, S1_Pin, GPIO_PinState((input >> 1) & 1));
	HAL_GPIO_WritePin(GPIOB, S0_Pin, GPIO_PinState((input >> 0) & 1));
}
