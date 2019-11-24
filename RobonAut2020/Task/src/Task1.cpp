/*
 * Task1.cpp
 *
 *  Created on: Oct 23, 2019
 *      Author: Joci
 */

#include "Task1.h"

#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "ToF_2m_L0.h"
#include "ToF_4m_L1.h"

void Task1::Init_Task1()
{
    xTaskCreate(Task1::Task1_Process,
                "TASK1",
                1512,
                NULL,
                5,
                NULL);
}

void Task1::Task1_Process(void *pvParameters)
{
    /*uint8_t newI2C = 0x52;
    VL53L1_RangingMeasurementData_t RangingData;
    int status;
    VL53L1_Dev_t   dev;
    VL53L1_DEV     Dev = &dev;
    uint8_t        byteData;
    uint16_t       wordData;
    uint32_t       milimeter;

    HAL_GPIO_WritePin(FREE3_LED_GPIO_Port, FREE3_LED_Pin, GPIO_PIN_RESET);

    vTaskDelay(80);

    HAL_GPIO_WritePin(FREE3_LED_GPIO_Port, FREE3_LED_Pin, GPIO_PIN_SET);
    Dev->comms_speed_khz = 400;
    Dev->I2cHandle = &hi2c1;
    Dev->comms_type = 1;
    Dev->I2cDevAddr=0x52; // default ToF sensor I2C address
    VL53L1_RdWord(Dev, 0x010F, &wordData);
    newI2C = Dev->I2cDevAddr + (0+1)*2;
    status = VL53L1_SetDeviceAddress(Dev, newI2C);
    Dev->I2cDevAddr=newI2C;
    VL53L1_RdWord(Dev, 0x010F, &wordData);

    status = VL53L1_WaitDeviceBooted(Dev);
    status = VL53L1_DataInit(Dev);
    status = VL53L1_StaticInit(Dev);
    status = VL53L1_SetDistanceMode(Dev, VL53L1_DISTANCEMODE_LONG);
    status = VL53L1_SetMeasurementTimingBudgetMicroSeconds(Dev, 20000);
    status = VL53L1_SetInterMeasurementPeriodMilliSeconds(Dev, 200);
    status = VL53L1_StartMeasurement(Dev);*/

    TOF_L1 distSensor(0x52, 400, &hi2c1, 200, TOF_CS1_GPIO_Port, TOF_CS1_Pin, 0, 0);

    distSensor.Init();

    while (1)
    {
        U8 a = 0;

        distSensor.Process();

        a = distSensor.GetDistance_mm();

        /*status = VL53L1_StartMeasurement(Dev);
        status = VL53L1_WaitMeasurementDataReady(Dev);
        if(!status)
        {
            // DEB
            HAL_GPIO_WritePin(FREE2_GPIO_Port, FREE2_Pin, GPIO_PIN_SET);

            status = VL53L1_GetRangingMeasurementData(Dev, &RangingData);
            if(status==0)
            {
                milimeter = RangingData.RangeMilliMeter;
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
        }*/

        vTaskDelay(20);
    }
}
