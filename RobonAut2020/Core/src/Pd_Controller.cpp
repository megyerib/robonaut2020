/*
 * Pd_Controller.cpp
 *
 *  Created on: Nov 17, 2019
 *      Author: Joci
 */

#include "Pd_Controller.h"

Pd_Controller::Pd_Controller(void)
{
    Controller();
    kd             = 0.0f;
    derivative     = 0.0f;
    previous_error = 0.0f;
}

Pd_Controller::Pd_Controller(float Kp, float Kd)
{
    Pd_Controller();
    kp = Kp;
    kd = Kd;
}

void Pd_Controller::Process(float processValue)
{
    // e(t)
    CalculateErrorValue(processValue);

    // d/dt e(t)
    derivative = loop.error_value - previous_error;

    // u(t) = P * e(t) + D * d/dt e(t)
    loop.control_value = kp * loop.error_value + kd * derivative;

    // Actualize.
    previous_error = loop.error_value;
}

void Pd_Controller::Set_D_Term(float D)
{
    kd = D;
}
