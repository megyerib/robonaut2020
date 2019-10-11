#include "trackdrawer.h"
#include <QPainter>
#include <QRect>

#define REFRESH_INTERVAL 40 /* [ms] -> 25 FPS */

TrackDrawer::TrackDrawer(QWidget &w, SimSetting& s) :
    widget(w),
    setting(s)
{
    // Initial state: Image in the top left, original resolution

    trackWidth  = double(setting.trackImg.width()) / setting.pixelPerMeter;
    trackHeight = double(setting.trackImg.height()) / setting.pixelPerMeter;

    WindowCs = new CartesianCS
    (
        0,
        trackHeight,
        0,
        +1.0 / setting.pixelPerMeter,
        -1.0 / setting.pixelPerMeter  // Y inverted
    );

    BgCs = new CartesianCS
    (
        0,
        trackHeight,
        0,
        +1.0 / setting.pixelPerMeter,
        -1.0 / setting.pixelPerMeter  // Y inverted
    );

    setting.vrobot->PassTrackParameters(widget, setting.trackImg, BgCs, WindowCs);
    setting.vrobot->PostCfg();

    // Set refresh timer
    refreshTimer.setInterval(REFRESH_INTERVAL);
    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    refreshTimer.start();
}

void TrackDrawer::Draw()
{
    QPainter painter(&widget);
    //painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QImage& bg = setting.trackImg;

    QRect sourceRect(0, 0, bg.width(), bg.height()); // Constant

    static CartesianLoc topLeft(WORLD, 0, trackHeight);
    static CartesianLoc botRight(WORLD, trackWidth, 0);

    CartesianLoc topLeftWindow(topLeft);
    topLeftWindow.TransformTo(WindowCs);

    CartesianLoc botRightWindow(botRight);
    botRightWindow.TransformTo(WindowCs);

    QPoint imgTopLeft(int(topLeftWindow.x()), int(topLeftWindow.y()));
    QPoint imgBotRight(int(botRightWindow.x()), int(botRightWindow.y()));

    QRect targetRect(imgTopLeft, imgBotRight);

    painter.drawImage(targetRect, bg, sourceRect);

    setting.vrobot->Draw();
}

void TrackDrawer::Drag(QPoint &start, QPoint &end)
{
    CartesianLoc move(WindowCs, -(end.x() - start.x()), -(end.y() - start.y())); // New midpoint
    move.TransformTo(WORLD);

    WindowCs->center_x = move.x();
    WindowCs->center_y = move.y();

    widget.repaint();
}

void TrackDrawer::Zoom(QPoint &center, double magnitude)
{
    // New midpoint

    CartesianLoc move
    (
        WindowCs,
        double(center.x()) * (magnitude - 1.0) / magnitude, // X
        double(center.y()) * (magnitude - 1.0) / magnitude  // Y
    );

    move.TransformTo(WORLD);

    WindowCs->center_x = move.x();
    WindowCs->center_y = move.y();

    WindowCs->x_res /= magnitude;
    WindowCs->y_res /= magnitude;

    WindowCs->RecalcBaseVectors();

    widget.repaint();
}

void TrackDrawer::Refresh()
{
    widget.repaint();
}
