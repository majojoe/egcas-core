#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T19:54:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = egCAS
TEMPLATE = app

CONFIG += c++11

LIBS += -legcas

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
        src/view/formulaitem.cpp

HEADERS  += src/mainwindow.h \
        src/view/formulaitem.h

FORMS    += src/ui/mainwindow.ui
