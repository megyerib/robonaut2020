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
#include "Pd_Controller.h"

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
    TOF_L1 distSensor(0x52, 400, &hi2c1, 200, TOF_CS1_GPIO_Port, TOF_CS1_Pin);
    Pd_Controller dist_ctrl(0.5f, 0.1f);

    distSensor.Init();
    dist_ctrl.SetSetpoint(300);

    while (1)
    {
        S16 a = 0;

        distSensor.Process();

        a = distSensor.GetDistance_mm();

        dist_ctrl.Process(a);

        // CV must be negated to keep the wanted distance.
        if ((dist_ctrl.GetControlValue() * -1) > 0)
        {
            HAL_GPIO_WritePin(FREE3_LED_GPIO_Port, FREE3_LED_Pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(FREE3_LED_GPIO_Port, FREE3_LED_Pin, GPIO_PIN_SET);
        }


        vTaskDelay(20);
    }
}
