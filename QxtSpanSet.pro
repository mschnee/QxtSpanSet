#-------------------------------------------------
#
# Project created by QtCreator 2011-05-26T10:08:23
#
#-------------------------------------------------

QT       += core gui

TARGET = QxtSpanSet
CONFIG   += console
CONFIG   -= app_bundle
QMAKE_CXXFLAGS += -std=c++0x #officially c++11 now, but Qt 4.7 doesn't know that...
TEMPLATE = app

DEFINES += QT_4
SOURCES += main.cpp

HEADERS += \
    qxtspanset.h
