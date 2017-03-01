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
    setsettings.cpp \
    runexternalcommand.cpp

HEADERS  += mainwindow.h \
    setsettings.h \
    editcommand.h \
    runexternalcommand.h

FORMS    += mainwindow.ui \
    setsettings.ui \
    editcommand.ui

RESOURCES += \
    images.qrc

RC_ICONS = images/icon.ico

# The application version
VERSION = 0.0.0.3

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

#@VERSION = 0.0.0.3
QMAKE_TARGET_COMPANY = VioD
QMAKE_TARGET_PRODUCT = IpClip
#QMAKE_TARGET_DESCRIPTION = description
#QMAKE_TARGET_COPYRIGHT = copyright@

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DISTFILES +=
