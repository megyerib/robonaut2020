#pragma once

#include "Controller.h"

class Pid_Controller : public Controller
{
private:
    float ki;
    float kd;
    float integrate;
    float derivative;
    float previous_error;

public:
    Pid_Controller();
    Pid_Controller(float const Kp, float const Ki, float const Kd);

    void Process(float const porcessValue) override;

    void Set_I_Term(float const I);
    void Set_D_Term(float const D);
};
