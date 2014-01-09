#-------------------------------------------------
#
# Project created by QtCreator 2012-09-07T15:59:59
#
#-------------------------------------------------

# this needs to stay in sync with the Makefile
VERSION = 1.0.5

QT       += network declarative multimedia phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qml-viewer
TEMPLATE = app

# add #define for the version
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp\
    connection.cpp \
    mainview.cpp \
    messagehandler.cpp \
    module.cpp

HEADERS  += \
    connection.h \
    mainview.h \
    messagehandler.h \
    systemdefs.h \
    module.h

