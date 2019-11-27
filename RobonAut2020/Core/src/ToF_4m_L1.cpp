/*
 * ToF_4m_L1.cpp
 *
 *  Created on: 2019. nov. 24.
 *      Author: Joci
 */

#include "ToF_4m_L1.h"
#include "main.h"
#include "i2c.h"

TOF_L1::TOF_L1()
{
    Dev->I2cDevAddr      = 0x52;
    Dev->I2cHandle       = &hi2c1;
    Dev->comms_speed_khz = 400;
    Dev->comms_type      = 1;

    status          = VL53L1_ERROR_NONE;
    timingBudget_ms = 200;

    XSDN_Port = TOF_CS1_GPIO_Port;
    XSDN_Pin  = TOF_CS1_Pin;
}

TOF_L1::TOF_L1(U8                   const Addr,
               U16                  const Speed,
               I2C_HandleTypeDef*   const Hi2c,
               U32                  const TB_ms,
               GPIO_TypeDef*        const XsdnPort,
               U16                  const XsdnPin)
{
    Dev->I2cDevAddr      = Addr;
    Dev->I2cHandle       = Hi2c;
    Dev->comms_speed_khz = Speed;
    Dev->comms_type      = 1;

    status          = VL53L1_ERROR_NONE;
    timingBudget_ms = TB_ms;

    XSDN_Port = XsdnPort;
    XSDN_Pin  = XsdnPin;
}

void TOF_L1::Init()
{
    U16 newAddr = Dev->I2cDevAddr;
    U16 wordData = 0x00;

    // Shutdown sensor.
    HAL_GPIO_WritePin(XSDN_Port, XSDN_Pin, GPIO_PIN_RESET);
    vTaskDelay(80);
    // Start the sensor.
    HAL_GPIO_WritePin(XSDN_Port, XSDN_Pin, GPIO_PIN_SET);

    // default sensor I2C address.
    Dev->I2cDevAddr = 0x52;

    // Test if the device is available on the I2C network.
    status = VL53L1_RdWord(Dev, 0x010F, &wordData);
    if ((status == VL53L1_ERROR_NONE) && (wordData == 0xEACC))
    {
        // Set new address.
        Dev->I2cDevAddr = newAddr;
        status = VL53L1_SetDeviceAddress(Dev, Dev->I2cDevAddr);

        // Test I2C interface.
        status = VL53L1_RdWord(Dev, 0x0110, &wordData);

        if (wordData == 0xCC10)
        {
            status = VL53L1_WaitDeviceBooted(Dev);
            status = VL53L1_DataInit(Dev);
            status = VL53L1_StaticInit(Dev);
            status = VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
            status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, timingBudget_ms * 100);
            status = VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, timingBudget_ms);
            status = VL53L1_StartMeasurement(Dev);
        }
        else
        {
            // Reference register value is wrong.
            status = VL53L1_ERROR_CONTROL_INTERFACE;
        }
    }
}

void TOF_L1::Process()
{
    status = VL53L1_StartMeasurement(Dev);
    status = VL53L1_WaitMeasurementDataReady(Dev);
    if(status == VL53L1_ERROR_NONE)
    {
        // DEB
        HAL_GPIO_WritePin(FREE2_GPIO_Port, FREE2_Pin, GPIO_PIN_SET);

        status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
        if (status == VL53L1_ERROR_NONE)
        {
            U16 milimeter = RangingData.RangeMilliMeter;
            if (milimeter > 300)
            {
                HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_SET);
            }
            else
            {
                HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_RESET);
            }
        }
        else
        {
            HAL_GPIO_WritePin(FREE1_GPIO_Port, FREE1_Pin, GPIO_PIN_RESET);
        }
        status = VL53L1_ClearInterruptAndStartMeasurement(Dev);
    }
    else
    {
        // DEB
        HAL_GPIO_WritePin(FREE2_GPIO_Port, FREE2_Pin, GPIO_PIN_RESET);
    }
}

S16 TOF_L1::GetDistance_mm(void)
{
    return RangingData.RangeMilliMeter;
}

void TOF_L1::Calibrate_Offset_300mm(void)
{
    VL53L1_PerformOffsetCalibration(Dev, 300);
}


