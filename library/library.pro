#-------------------------------------------------
#
# Project created by QtCreator 2016-09-24T20:10:26
#
#-------------------------------------------------

QT       -= gui

TARGET = library
TEMPLATE = lib
CONFIG += staticlib

SOURCES += relayctl.cpp \
    denkovi.cpp

HEADERS += relayctl.h \
    denkovi.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
