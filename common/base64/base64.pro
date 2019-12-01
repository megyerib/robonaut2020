TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ../src/base64.c \
        main.c

HEADERS += \
    ../inc/base64.h
