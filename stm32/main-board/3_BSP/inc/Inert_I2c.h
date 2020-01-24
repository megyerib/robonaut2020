#pragma once

#include "stm32f4xx_hal.h"

class INERT_I2C
{
public:
    static INERT_I2C* GetInstance();
    I2C_HandleTypeDef* GetHandle();

private:
    I2C_HandleTypeDef* handle;

    INERT_I2C();
    void Init();
    void ConfigureHandle();
    void ConfigureGpio();
};
