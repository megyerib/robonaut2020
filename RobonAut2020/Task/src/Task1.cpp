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

#include "vl53l1_api.h"
#include "vl53l1_platform.h"
#include "vl53l1_def.h"

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
    ToF_2m tof;
    HAL_I2C_StateTypeDef state  = HAL_I2C_STATE_READY;
   // HAL_StatusTypeDef    status = HAL_OK;
    uint8_t pData[15];
    uint8_t dummyData[] = {0x01, 0x00, 0x0A, 0xFF};
    uint8_t regs[] = {0xC0, 0xC1, 0xC2};

    tof.Init();

    ////////////

    uint8_t newI2C = 0x52;
    VL53L1_RangingMeasurementData_t RangingData;
    int status;
    VL53L1_Dev_t   dev;
    VL53L1_DEV     Dev = &dev;
    uint8_t        byteData;
    uint16_t       wordData;
    uint32_t       milimeter;

/*    VL53L1_RdByte(Dev, 0x010F, &byteData);
    VL53L1_RdByte(Dev, 0x0110, &byteData);
    VL53L1_RdWord(Dev, 0x010F, &wordData);*/
    HAL_GPIO_WritePin(FREE3_LED_GPIO_Port, FREE3_LED_Pin, GPIO_PIN_RESET);

    vTaskDelay(80);

    HAL_GPIO_WritePin(FREE3_LED_GPIO_Port, FREE3_LED_Pin, GPIO_PIN_SET);
    Dev->comms_speed_khz = 400;
    Dev->I2cHandle = &hi2c1;
    Dev->comms_type = 1;
    Dev->I2cDevAddr=0x52; /* default ToF sensor I2C address*/
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
    status = VL53L1_StartMeasurement(Dev);

//    /*status = HAL_I2C_Mem_Write(&hi2c1, 0x29, 0x88, 1, 0x00, 1, HAL_MAX_DELAY);
//    status = HAL_I2C_Mem_Write(&hi2c1, 0x30, 0x88, 1, 0x00, 1, HAL_MAX_DELAY);
//    status = HAL_I2C_Mem_Write(&hi2c1, 0x31, 0x88, 1, 0x00, 1, HAL_MAX_DELAY);
//
//    status = HAL_I2C_Mem_Read(&hi2c1, 0x52, 0xC0, 1, pData+2, 1, HAL_MAX_DELAY);*/
//
//    //status = HAL_I2C_Master_Receive(&hi2c1, 0x08, pData+5, 1, HAL_MAX_DELAY);
//    //status = HAL_I2C_Mem_Read_IT(&hi2c1, 0x52, 0xC0, 1, pData+2, 1);
//    //status = HAL_I2C_Mem_Write(&hi2c1, 0x53, 0x00, 1, dummyData, 1, 500);

    //HAL_I2C_Mem_Write(&hi2c1, 0x52, 0x88, 1, dummyData+2, 1, 50000);

    while (1)
    {
        vTaskDelay(20);

        status = VL53L1_StartMeasurement(Dev);
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
        }


//        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//        HAL_I2C_Mem_Read(&hi2c1, 0x52, 0xC0, 1, pData+2, 1, HAL_MAX_DELAY);
//        tof.GetDevcInfo();
//        status = HAL_I2C_Master_Receive(&hi2c1, 0x08, pData+5, 6, HAL_MAX_DELAY);
//        status = HAL_I2C_Master_Receive(&hi2c3, 0x68, pData+13, 2, HAL_MAX_DELAY);
//        status = HAL_I2C_Mem_Read(&hi2c3, 0x68, 0x0D, 1, pData+2, 1, HAL_MAX_DELAY);
//        status = HAL_I2C_Mem_Write(&hi2c1, 0x10, 0x00, 1, dummyData, 1, HAL_MAX_DELAY);
//        status = HAL_I2C_Master_Transmit(&hi2c1, 0x10, dummyData, 1, 500);
//        status = HAL_I2C_Mem_Read_IT(&hi2c1, 0x52, 0xC0, 1, pData+2, 1);

        //state = HAL_I2C_GetState(&hi2c1);

       /* if (state == HAL_I2C_STATE_READY)
        {
            //status = HAL_I2C_Mem_Read(&hi2c1, 0x52, regs[1], 1, pData+3, 1, 50000);
            //tof.GetDistance();
//            status = HAL_I2C_Mem_Write(&hi2c1, 0x52, regs[1], 1, pData+5, 1, 50000);
        }

        state = HAL_I2C_GetState(&hi2c1);


        if (status == HAL_ERROR)
        {
           // HAL_I2C_Master_Abort_IT(&hi2c1, 0x52);
        }

        VL53L1_RdByte(Dev, 0x010F, &byteData);
        vTaskDelay(200);
        VL53L1_RdByte(Dev, 0x0110, &byteData);
        vTaskDelay(200);
        VL53L1_RdWord(Dev, 0x010F, &wordData);
        vTaskDelay(200);
        VL53L1_ClearInterruptAndStartMeasurement(Dev);
        VL53L1_GetRangingMeasurementData(Dev, &tavolsag);
        vTaskDelay(200);*/


    }
}
