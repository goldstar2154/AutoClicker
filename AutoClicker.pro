#-------------------------------------------------
#
# Project created by QtCreator 2017-01-25T22:21:26
#
#-------------------------------------------------

QT       += core gui
INCLUDEPATH += /usr/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoClicker
TEMPLATE = app

include(qhotkey.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    clicker.cpp

HEADERS  += mainwindow.h \
    clicker.h

LIBS += -lXtst

FORMS    += mainwindow.ui
