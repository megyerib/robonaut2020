#pragma once

#include "CyclicTask.h"
#include "Navigation.h"

class NavigationTask : public CyclicTask
{
    Navigation* navigation;

    NavigationTask();

    void TaskInit() override;
    void TaskFunction() override;

public:
    static NavigationTask* Init();
};
