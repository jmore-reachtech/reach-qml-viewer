#-------------------------------------------------
#
# Project created by QtCreator 2012-09-07T15:59:59
#
#-------------------------------------------------

QT       += network declarative

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qml-viewer
TEMPLATE = app


SOURCES += main.cpp\
    connection.cpp \
    mainview.cpp \
    messagehandler.cpp

HEADERS  += \
    connection.h \
    mainview.h \
    messagehandler.h \
    systemdefs.h

