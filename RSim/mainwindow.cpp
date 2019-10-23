#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "simulation/simulation.h"
#include "simulation/mazesim1.h"

#define REFRESH_INTERVAL 0.04

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);

    // Initialize simulation
    simulation = dynamic_cast<Simulation*>(new MazeSim1(*this));

    // Set timer
    refreshTimer.setInterval(int(REFRESH_INTERVAL*1000));
    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    refreshTimer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    simulation->Draw();
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
        simulation->display->Drag(start, end);
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

    simulation->display->Zoom(center, zoom);
}

void MainWindow::Refresh()
{
    repaint();
}
