/*
 * _Task_Qualification.cpp
 *
 *  Created on: Nov 17, 2019
 *      Author: Joci
 */

#include "_Task_Qualification.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Car.h"

void Task_Qualification::Init_Task_Qualification()
{
   /* xTaskCreate(Task_Qualification::Task_Qualification_Process,
                "TASK_QUALI",
                512,
                NULL,
                5,
                NULL);*/
}

void Task_Qualification::Task_Qualification_Process(void *pvParameters)
{
    Car robot;

    while (1)
    {
        robot.Process_Quali();
    }
}

