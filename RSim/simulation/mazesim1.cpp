#include "mazesim1.h"

#define INITIAL_X 8.350
#define INITIAL_Y 3.236
#define INITIAL_PHI DEG_TO_RAD(90)
#define GATE_NUM 12

static CartesianPos gates[GATE_NUM] =
{
    CartesianPos(WORLD, 8.350, 3.236, DEG_TO_RAD(0)),
    CartesianPos(WORLD, 7.016, 9.350, DEG_TO_RAD(90)),
    CartesianPos(WORLD, 7.033, 4.463, DEG_TO_RAD(90)),
    CartesianPos(WORLD, 5.894, 2.748, DEG_TO_RAD(0)),
    CartesianPos(WORLD, 5.902, 5.268, DEG_TO_RAD(0)),
    CartesianPos(WORLD, 5.894, 7.789, DEG_TO_RAD(0)),
    CartesianPos(WORLD, 4.659, 9.439, DEG_TO_RAD(90)),
    CartesianPos(WORLD, 5.285, 6.130, DEG_TO_RAD(-35)),
    CartesianPos(WORLD, 3.585, 3.667, DEG_TO_RAD(90)),
    CartesianPos(WORLD, 3.732, 6.285, DEG_TO_RAD(90)),
    CartesianPos(WORLD, 2.236, 3.992, DEG_TO_RAD(0)),
    CartesianPos(WORLD, 2.276, 2.057, DEG_TO_RAD(0))
};

MazeSim1::MazeSim1(QWidget& w)
{
    display = new Display(w);

    track   = new Track(*display, "../RSim/resource/maze2019.png", 123);

    CartesianPos initialRobotPos(WORLD, INITIAL_X, INITIAL_Y, INITIAL_PHI); // It will be copied so local scope is no problem.
    robot   = new SimRobot1(display, track, &initialRobotPos);

    for (uint i = 0; i < GATE_NUM; i++)
        checkpoints.push_back(new Checkpoint(*display, gates[i], *robot));
}

void MazeSim1::Draw()
{
    track->Draw();
    robot->Draw();

    for (uint i = 0; i < checkpoints.size(); i++)
    {
        checkpoints[i]->Draw();
    }
}
