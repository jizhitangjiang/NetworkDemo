#-------------------------------------------------
#
# Project created by QtCreator 2019-03-11T19:18:30
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = res/image/app.ico
#RC_FILE = networkdemo.rc
win32::QMAKE_CXXFLAGS += -FIcharsetsetting.inc
DESTDIR = ../bin
TARGET = NetworkDemo

INCLUDEPATH += src/ui\
               src/utils

SOURCES += main.cpp\
    src/utils/Utils.cpp \
    src/utils/SystemTrayIcon.cpp \
    src/utils/WindowStyle.cpp \
    src/ui/MainWindow.cpp \
    src/utils/HostInfo.cpp \
    src/utils/NetWork.cpp \
    src/ui/DownloadItemForm.cpp \
    src/utils/FileHandler.cpp

HEADERS  += \
    src/utils/Utils.h \
    src/utils/SystemTrayIcon.h \
    src/utils/WindowStyle.h \
    src/ui/MainWindow.h \
    src/utils/HostInfo.h \
    src/utils/NetWork.h \
    src/ui/DownloadItemForm.h \
    src/utils/FileHandler.h

FORMS    += \
    src/ui/MainWindow.ui \
    src/ui/DownloadItemForm.ui

RESOURCES += \
    qrc.qrc
