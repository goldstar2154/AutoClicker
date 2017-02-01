#-------------------------------------------------
#
# Project created by QtCreator 2017-01-25T22:21:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoClicker
TEMPLATE = app

include(qhotkey.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
