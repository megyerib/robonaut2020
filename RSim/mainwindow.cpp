#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trackdrawer.h"
#include "simsetting.h"
#include <QDebug>
#include "simrobot1.h"

static RobotProxy* robot;
static WidgetDrawer* drawer;
static SimSetting* setting;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    robot  = new SimRobot1();
    setting = new SimSetting("track01.png", 200, robot);
    drawer = new TrackDrawer(*this, *setting);

    // Img refresh!!
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

static bool moveInProgress = false;

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    static QPoint start, end;

    if (!(event->buttons() & Qt::LeftButton))
        return;

    end.setX(event->x());
    end.setY(event->y());

    if (moveInProgress)
    {
        drawer->Drag(start, end);
    }

    start = end;

    moveInProgress = true;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    moveInProgress = false;
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    // Mouse: +/-120; Touchpad: Much smaller(2+)

    QPoint center = event->pos();
    double zoom = 1.0 + event->delta() / 1200.0; // 120: 0.1* zoom

    drawer->Zoom(center, zoom);
}
