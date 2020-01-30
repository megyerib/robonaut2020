/*
 * TaskRemote.h
 *
 *  Created on: 2019. nov. 16.
 *      Author: Joci
 */

#ifndef TASKREMOTE_H_
#define TASKREMOTE_H_

class Task_Remote
{
public:
    static void Init_Task_Remote();
    static void Task_Remote_Process(void *pvParameters);
};


#endif /* TASKREMOTE_H_ */
