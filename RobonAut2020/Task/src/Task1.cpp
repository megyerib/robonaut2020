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

void Task1::Init_Task1()
{
    xTaskCreate(Task1::Task1_Process,
                "TASK1",
                512,
                NULL,
                5,
                NULL);
}

void Task1::Task1_Process(void *pvParameters)
{
    ToF_2m tof;
    HAL_I2C_StateTypeDef state  = HAL_I2C_STATE_READY;
    HAL_StatusTypeDef    status = HAL_OK;
    uint8_t pData[15];
    uint8_t dummyData[] = {0x01, 0x00, 0x0A, 0xFF};
    uint8_t regs[] = {0xC0, 0xC1, 0xC2};

    tof.Init();

    vTaskDelay(100);

    /*status = HAL_I2C_Mem_Write(&hi2c1, 0x29, 0x88, 1, 0x00, 1, HAL_MAX_DELAY);
    status = HAL_I2C_Mem_Write(&hi2c1, 0x30, 0x88, 1, 0x00, 1, HAL_MAX_DELAY);
    status = HAL_I2C_Mem_Write(&hi2c1, 0x31, 0x88, 1, 0x00, 1, HAL_MAX_DELAY);

    status = HAL_I2C_Mem_Read(&hi2c1, 0x52, 0xC0, 1, pData+2, 1, HAL_MAX_DELAY);*/

    //status = HAL_I2C_Master_Receive(&hi2c1, 0x08, pData+5, 1, HAL_MAX_DELAY);
    //status = HAL_I2C_Mem_Read_IT(&hi2c1, 0x52, 0xC0, 1, pData+2, 1);
    //status = HAL_I2C_Mem_Write(&hi2c1, 0x53, 0x00, 1, dummyData, 1, 500);

    HAL_I2C_Mem_Write(&hi2c1, 0x52, 0x88, 1, dummyData+2, 1, 50000);

    while (1)
    {
       // HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        vTaskDelay(2000);
        //HAL_I2C_Mem_Read(&hi2c1, 0x52, 0xC0, 1, pData+2, 1, HAL_MAX_DELAY);
        //tof.GetDevcInfo();
        //status = HAL_I2C_Master_Receive(&hi2c1, 0x08, pData+5, 6, HAL_MAX_DELAY);
        //status = HAL_I2C_Master_Receive(&hi2c3, 0x68, pData+13, 2, HAL_MAX_DELAY);
        //status = HAL_I2C_Mem_Read(&hi2c3, 0x68, 0x0D, 1, pData+2, 1, HAL_MAX_DELAY);
       // status = HAL_I2C_Mem_Write(&hi2c1, 0x10, 0x00, 1, dummyData, 1, HAL_MAX_DELAY);
        //status = HAL_I2C_Master_Transmit(&hi2c1, 0x10, dummyData, 1, 500);
       // status = HAL_I2C_Mem_Read_IT(&hi2c1, 0x52, 0xC0, 1, pData+2, 1);

        state = HAL_I2C_GetState(&hi2c1);

        if (state == HAL_I2C_STATE_READY)
        {
            status = HAL_I2C_Mem_Read(&hi2c1, 0x52, regs[1], 1, pData+3, 1, 50000);
            tof.GetDistance();
            //status = HAL_I2C_Mem_Write(&hi2c1, 0x52, regs[1], 1, pData+5, 1, 50000);
        }

        state = HAL_I2C_GetState(&hi2c1);


        if (status == HAL_ERROR)
        {
           // HAL_I2C_Master_Abort_IT(&hi2c1, 0x52);
        }

    }
}
