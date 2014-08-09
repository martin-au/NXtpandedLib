#-------------------------------------------------
#
# Project created by QtCreator 2013-12-21T18:29:17
#
# Build : MSVC 2012
# Qt Creator Version 5.2
# Fantom driver 1.1.2
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = nxtosekUsbTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += _CRT_SECURE_NO_WARNINGS

LIBS += -L$$PWD/fantom/targets/win32U/i386/msvc71/release/ -lfantom

INCLUDEPATH += $$PWD/fantom/includes
DEPENDPATH += $$PWD/fantom/includes

PRE_TARGETDEPS += $$PWD/fantom/targets/win32U/i386/msvc71/release/fantom.lib


SOURCES += main.cpp \
    nxtcommfantom.cpp \
    ncom.cpp \
    logg.cpp

HEADERS += \
    nxtcommfantom.h \
    ncom.h \
    logg.h \
