QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    refreshable.cpp \
    robot/simrobot1.cpp \
    sensor/linesensor.cpp \
    sensor/linesensoravg.cpp \
    simulation/checkpoint.cpp \
    simulation/demosim.cpp \
    simulation/display.cpp \
    simulation/mazesim1.cpp \
    simulation/track.cpp \
    main.cpp \
    mainwindow.cpp \
    navimath2d.cpp

HEADERS += \
    refreshable.h \
    robot/robot.h \
    robot/simrobot1.h \
    robot/virtualrobot.h \
    sensor/linesensor.h \
    sensor/linesensoravg.h \
    simulation/checkpoint.h \
    simulation/demosim.h \
    simulation/display.h \
    simulation/mazesim1.h \
    simulation/simulation.h \
    simulation/track.h \
    mainwindow.h \
    navimath2d.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
