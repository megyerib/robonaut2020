/*
 * Remote.h
 *
 *  Created on: Nov 16, 2019
 *      Author: Joci
 */

#ifndef REMOTE_H_
#define REMOTE_H_

#include "OVERRIDE_Types.h"
#include "Timer.h"

#define MIDDLE  2945 /* us */
#define MAX     4060 /* us */

#define MARGIN   250 /* us */

#define ON_THR     (MIDDLE + MARGIN)
#define UPPER_THR  (MAX + MARGIN)

enum RemoteState
{
    Invalid = 0,
    Pressed,
    Released
};

class Remote
{
private:
    static U8 State;
    U8 valami;

    Timer* timCh1;
    Timer* timCh2;
    Timer* timCh3;
    Timer* timCh4;

public:
    Remote();

    U8 GetState(U32 Channel);

    static void IC_IT(TIM_HandleTypeDef *htim);
};

#endif /* REMOTE_H_ */
