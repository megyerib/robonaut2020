#pragma once

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
