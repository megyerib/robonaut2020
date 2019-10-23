#include "demosim.h"

#define INITIAL_X 2.5
#define INITIAL_Y 16.48
#define INITIAL_PHI DEG_TO_RAD(180)

DemoSim::DemoSim(QWidget& w)
{
    display = new Display(w);

    track   = new Track(*display, "../RSim/resource/track_q_sprint.png", 131);

    CartesianPos initialRobotPos(WORLD, INITIAL_X, INITIAL_Y, INITIAL_PHI); // It will be copied so local scope is no problem.
    robot   = new SimRobot1(display, &initialRobotPos);
}

void DemoSim::Draw()
{
    track->Draw();
    robot->Draw();
}
