/*
 * TaskRemote.cpp
 *
 *  Created on: 2019. nov. 16.
 *      Author: Joci
 */

#include "Task_Remote.h"

#include "main.h"
#include "Remote.h"

#include "FreeRTOS.h"
#include "task.h"


void Task_Remote::Init_Task_Remote()
{
    /*xTaskCreate(Task_Remote::Task_Remote_Process,
                "TASK_REMOTE",
                512,
                NULL,
                5,
                NULL);*/
}

void Task_Remote::Task_Remote_Process(void *pvParameters)
{
    Remote carRemote;
    U8 st = 10;

    while (1)
    {
        st = carRemote.GetState(st);
    }
}



