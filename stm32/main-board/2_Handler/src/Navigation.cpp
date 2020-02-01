#include "Navigation.h"

Navigation* Navigation::GetInstance()
{
    static Navigation instance;
    return &instance;
}

void Navigation::Process()
{
    inert->Process();
}

Navigation::Navigation()
{
    inert = new LSM6DSO();

    inert->Init();
}
