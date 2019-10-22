#include "demosim.h"

DemoSim::DemoSim(QWidget& w)
{
    display = new Display(w);
    track   = new Track(*display, "../RSim/resource/track_q_sprint.png", 131);
    //robot   = new SimRobot1();
}

void DemoSim::Draw()
{
    track->Draw();
}
