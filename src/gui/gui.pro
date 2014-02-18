include(../../common.pri)

TEMPLATE = app

QT += declarative network

SOURCES += main.cpp

LIBS += -L../core -lrtcore

TARGET = qml-viewer
