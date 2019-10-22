#include "track.h"
#include <QPainter>

Track::Track(Display& d,
             const char* imgPath,
             int pixelPerMeter)
    :
      display(d),
      bgImg(imgPath),
      bgRes(pixelPerMeter)
{
    // Initial state: Image in the top left, original resolution

    width  = double(bgImg.width()) / bgRes;
    height = double(bgImg.height()) / bgRes;

    bgCs = new CartesianCS
    (
        0,
        height,
        0,
        +1.0 / bgRes,
        -1.0 / bgRes  // Y inverted
    );

    display.SetCS(bgCs);
}

void Track::Draw()
{
    QPainter painter(&display.widget);
    //painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QRect sourceRect(0, 0, bgImg.width(), bgImg.height()); // Constant

    static CartesianLoc topLeft(WORLD, 0, height);
    static CartesianLoc botRight(WORLD, width, 0);

    CartesianLoc topLeftWindow(topLeft);
    topLeftWindow.TransformTo(&display.cs);

    CartesianLoc botRightWindow(botRight);
    botRightWindow.TransformTo(&display.cs);

    QPoint imgTopLeft(int(topLeftWindow.x()), int(topLeftWindow.y()));
    QPoint imgBotRight(int(botRightWindow.x()), int(botRightWindow.y()));

    QRect targetRect(imgTopLeft, imgBotRight);

    painter.drawImage(targetRect, bgImg, sourceRect);
}
