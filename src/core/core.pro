include(../../common.pri)

QT += network declarative

HEADERS += connection.h \
        messagehandler.h \
        mainview.h 

SOURCES += connection.cpp \
        messagehandler.cpp \
        mainview.cpp 

TARGET = rtcore
