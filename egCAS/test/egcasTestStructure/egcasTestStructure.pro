#-------------------------------------------------
#
# Project created by QtCreator 2014-08-03T11:30:59
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = tst_egcastest_structure
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11

#LIBS += -legcas


SOURCES += tst_egcastest_view.cpp \
           ../../src/structure/formulaitem.cpp \
    ../../src/structure/egcasscene.cpp

HEADERS += ../../src/structure/formulaitem.h \
    ../../src/structure/egcasscene.h



DEFINES += SRCDIR=\\\"$$PWD/\\\"
