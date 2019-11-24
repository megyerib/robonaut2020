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
    TOF_L1 distSensor(0x52, 400, &hi2c1, 200, TOF_CS1_GPIO_Port, TOF_CS1_Pin, 0, 0);

    distSensor.Init();

    while (1)
    {
        U8 a = 0;

        distSensor.Process();

        a = distSensor.GetDistance_mm();

        vTaskDelay(20);
    }
}
