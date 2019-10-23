#include "demosim.h"

#define INITIAL_X 4
#define INITIAL_Y 16.48
#define INITIAL_PHI DEG_TO_RAD(180)
#define GATE_NUM 2

static CartesianPos gates[GATE_NUM] =
{
    CartesianPos(WORLD, 2.5, 16.48, DEG_TO_RAD(90)),
    CartesianPos(WORLD, 1.1, 16.2, DEG_TO_RAD(135))
};

DemoSim::DemoSim(QWidget& w)
{
    display = new Display(w);

    track   = new Track(*display, "../RSim/resource/track_q_sprint.png", 131);

    CartesianPos initialRobotPos(WORLD, INITIAL_X, INITIAL_Y, INITIAL_PHI); // It will be copied so local scope is no problem.
    robot   = new SimRobot1(display, track, &initialRobotPos);

    for (uint i = 0; i < GATE_NUM; i++)
        checkpoints.push_back(new Checkpoint(*display, gates[i], *robot));
}

void DemoSim::Draw()
{
    track->Draw();
    robot->Draw();

    for (uint i = 0; i < checkpoints.size(); i++)
    {
        checkpoints[i]->Draw();
    }
}
