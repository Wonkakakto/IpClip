#-------------------------------------------------
#
# Project created by QtCreator 2015-10-07T21:35:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IpClip
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editcommand.cpp \
    setsettings.cpp

HEADERS  += mainwindow.h \
    setsettings.h \
    editcommand.h

FORMS    += mainwindow.ui \
    setsettings.ui \
    editcommand.ui

RESOURCES += \
    images.qrc
