#-------------------------------------------------
#
# Project created by QtCreator 2016-03-29T16:57:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = parallel-tube
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    video.cpp \
    meta.cpp \
    youtube.cpp \
    networkmanager.cpp \
    videoinfo.cpp

HEADERS  += mainwindow.h \
    video.h \
    meta.h \
    youtube.h \
    networkmanager.h \
    videoinfo.h

FORMS    += mainwindow.ui \
    videoinfo.ui
