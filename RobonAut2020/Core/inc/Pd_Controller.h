/*
 * Pd_Controller.h
 *
 *  Created on: Nov 17, 2019
 *      Author: Joci
 */

#ifndef PD_CONTROLLER_H_
#define PD_CONTROLLER_H_

#include "Controller.h"

class Pd_Controller : public Controller
{
private:
    float kd;
    float derivative;
    float previous_error;

public:
    Pd_Controller();
    Pd_Controller(float Kp, float Kd);

    void Process(float processValue) override;

    void Set_D_Term(float D);
};


#endif /* PD_CONTROLLER_H_ */
