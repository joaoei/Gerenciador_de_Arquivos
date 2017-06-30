#-------------------------------------------------
#
# Project created by QtCreator 2017-06-24T18:25:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = proj_so
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    windowdir.cpp \
    aviso.cpp

HEADERS  += mainwindow.h \
    windowdir.h \
    aviso.h

FORMS    += mainwindow.ui \
    windowdir.ui \
    aviso.ui

CONFIG += c++11
