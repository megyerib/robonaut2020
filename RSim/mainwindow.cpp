#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trackdrawer.h"
#include "simsetting.h"
#include <QDebug>
#include "simrobot1.h"

static RobotProxy* robot;
static VirtualRobot* vrobot;
static TrackDrawer* drawer;
static SimSetting* setting;

static SimRobot1* simrobot;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    simrobot = new SimRobot1();

    robot  = static_cast<RobotProxy*>(simrobot);
    vrobot = static_cast<VirtualRobot*>(simrobot);
    setting = new SimSetting("../RSim/resource/track_q_sprint.png", 131, robot, vrobot);
    drawer = new TrackDrawer(*this, *setting);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    drawer->Draw();
}

static bool dragInProgress = false;

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    static QPoint start, end;

    if (!(event->buttons() & Qt::LeftButton))
        return;

    end.setX(event->x());
    end.setY(event->y());

    if (dragInProgress) // Before drag no start position was set
    {
        drawer->Drag(start, end);
    }

    start = end;

    dragInProgress = true;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    dragInProgress = false;
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    // Mouse: +/-120; Touchpad: Much smaller(2+)

    QPoint center = event->pos();
    double zoom = 1.0 + event->delta() / 1200.0; // 120: 0.1* zoom

    drawer->Zoom(center, zoom);
}
