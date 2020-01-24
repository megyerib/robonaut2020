#include "Inert_I2c.h"

INERT_I2C* INERT_I2C::GetInstance()
{
    static INERT_I2C instance;
    return &instance;
}

I2C_HandleTypeDef* INERT_I2C::GetHandle()
{
    return handle;
}

INERT_I2C::INERT_I2C()
{

}

void INERT_I2C::Init()
{
    // TODO
    //handle = &tof_handle;

    // I2C1 clock enable
    __HAL_RCC_I2C1_CLK_ENABLE();

    // I2C1 interrupt Init
    HAL_NVIC_SetPriority(I2C3_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
    HAL_NVIC_SetPriority(I2C3_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);

    //ConfigureHandle();

    //ConfigureGpio();
}

void INERT_I2C::ConfigureHandle()
{
    // TODO
    handle->Instance                 = I2C3;
    handle->Init.ClockSpeed          = 50000;
    handle->Init.DutyCycle           = I2C_DUTYCYCLE_2;
    handle->Init.OwnAddress1         = 0;
    handle->Init.AddressingMode      = I2C_ADDRESSINGMODE_7BIT;
    handle->Init.DualAddressMode     = I2C_DUALADDRESS_DISABLE;
    handle->Init.OwnAddress2         = 0;
    handle->Init.GeneralCallMode     = I2C_GENERALCALL_DISABLE;
    handle->Init.NoStretchMode       = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(handle) != HAL_OK)
    {
        //Error_Handler();
    }
}

void INERT_I2C::ConfigureGpio()
{
    // TODO
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    // I2C1 GPIO Configuration:
    //   PB6 ------> I2C1_SCL
    //   PB7 ------> I2C1_SDA
    //
    GPIO_InitStruct.Pin         = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull        = GPIO_PULLUP;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate   = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Init Debug GPIOSs (Free123)
    GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin         = GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3;
    GPIO_InitStruct.Mode        = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull        = GPIO_NOPULL;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
