/*
 * Car.h
 *
 *  Created on: Nov 17, 2019
 *      Author: Joci
 */

#ifndef CAR_H_
#define CAR_H_

#include "Remote.h"
#include "Steer.h"
#include "Distance.h"

enum RobotState
{
    Init = 0,
    Ideal,
    Follow,
    LineLost,
    Overtake,
    Stop
};

class Car
{
private:
    RobotState state;

    Remote        remote;
    Steer         wheel;
    Pd_Controller lineFollowController;

public:
    Car(void);

    void Process_Quali(void);

};



#endif /* CAR_H_ */
