#-------------------------------------------------
#
# Project created by QtCreator 2016-03-29T16:57:04
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = parallel-tube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    video.cpp \
    meta.cpp \
    youtube.cpp

HEADERS  += mainwindow.h \
    video.h \
    meta.h \
    youtube.h

FORMS    += mainwindow.ui
