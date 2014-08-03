#-------------------------------------------------
#
# Project created by QtCreator 2014-08-03T11:30:59
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = tst_egcastest_view
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11

LIBS += -legcas


SOURCES += tst_egcastest_view.cpp \
           ../../src/view/formulaitem.cpp \
    ../../src/view/egcasscene.cpp

HEADERS += ../../src/view/formulaitem.h \
    ../../src/view/egcasscene.h



DEFINES += SRCDIR=\\\"$$PWD/\\\"
