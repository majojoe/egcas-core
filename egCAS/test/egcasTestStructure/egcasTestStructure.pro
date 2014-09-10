#-------------------------------------------------
#
# Project created by QtCreator 2014-08-03T11:30:59
#
#-------------------------------------------------

QT       += widgets testlib

TARGET = tst_egcastest_structural
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11

SOURCES += tst_egcastest_structural.cpp \
           ../../src/structural/egcunaryexpressionnode.cpp \
           ../../src/structural/egcbinaryexpressionnode.cpp \
           ../../src/structural/egcexpressionnode.cpp \
           ../../src/structural/egcrootexpressionnode.cpp \
           ../../src/structural/egcnumberexpressionnode.cpp \
           ../../src/structural/egcexpressionnodecreator.cpp \
           ../../src/structural/egcvariableexpressionnode.cpp

HEADERS += ../../src/structural/egcunaryexpressionnode.h \
           ../../src/structural/egcbinaryexpressionnode.h \
           ../../src/structural/egcexpressionnode.h \
           ../../src/structural/egcrootexpressionnode.h \
           ../../src/structural/egcnumberexpressionnode.h \
           ../../src/structural/egcexpressonnodecreator.h \
           ../../src/structural/egcvariableexpressionnode.h \
           tst_egcastest_structural.h



DEFINES += SRCDIR=\\\"$$PWD/\\\"
