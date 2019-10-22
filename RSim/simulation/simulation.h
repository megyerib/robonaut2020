#pragma once

#include "track.h"

class Simulation
{
public:
    Display* display;

    virtual ~Simulation() = default;

    virtual void Draw() = 0;
};
