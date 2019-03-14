#-------------------------------------------------
#
# Project created by QtCreator 2019-03-11T19:18:30
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = res/image/app.ico
#RC_FILE = networkdemo.rc
win32::QMAKE_CXXFLAGS += -FIcharsetsetting.inc
DESTDIR = ../bin
MOC_DIR = ../Debug
TARGET = NetworkDemo1

INCLUDEPATH += src/ui\
               src/utils

SOURCES += main.cpp\
    src/ui/NetworkDemo.cpp \
    src/utils/Utils.cpp \
    src/utils/SystemTrayIcon.cpp

HEADERS  += \
    src/ui/NetworkDemo.h \
    src/utils/Utils.h \
    src/utils/SystemTrayIcon.h

FORMS    += \
    src/ui/NetworkDemo.ui

RESOURCES += \
    qrc.qrc
