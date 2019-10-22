#ifndef DEMOSIM_H
#define DEMOSIM_H

#include "simulation.h"
#include <QWidget>

#include "robot/simrobot1.h"
#include "track.h"

class DemoSim : public Simulation
{
    Track* track;
    //SimRobot1* robot;

public:
    DemoSim(QWidget& w);

    void Draw() override;
};

#endif // DEMOSIM_H
