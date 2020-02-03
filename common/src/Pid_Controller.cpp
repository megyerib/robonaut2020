#include "Pid_Controller.h"

Pid_Controller::Pid_Controller(void)
{
    Controller();
    ki             = 0.0f;
    kd             = 0.0f;
    integrate      = 0.0f;
    derivative     = 0.0f;
    previous_error = 0.0f;
}

Pid_Controller::Pid_Controller(float const Kp, float const Ki, float const Kd)
{
    Pid_Controller();
    kp = Kp;
    ki = Ki;
    kd = Kd;
}

void Pid_Controller::Process(float const processValue)
{
    // e(t)
    CalculateErrorValue(processValue);

    // d/dt e(t)
    derivative = loop.error_value - previous_error;

    // integrate( e(t) )
    integrate += loop.error_value;

    // u(t) = P * e(t) + I * integrate( e(t) ) + D * d/dt e(t)
    loop.control_value = kp * loop.error_value + ki * integrate + kd * derivative;

    // Actualize.
    previous_error = loop.error_value;
}

void Pid_Controller::Set_I_Term(float const I)
{
    ki = I;
}

void Pid_Controller::Set_D_Term(float const D)
{
    kd = D;
}
