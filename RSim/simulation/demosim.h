#pragma once

#include "simulation.h"
#include <QWidget>

#include "robot/simrobot1.h"
#include "simulation/checkpoint.h"
#include "track.h"

class DemoSim : public Simulation
{
    Track* track;
    SimRobot1* robot;
    std::vector<Checkpoint*> checkpoints;

public:
    DemoSim(QWidget& w);

    void Draw() override;
};
