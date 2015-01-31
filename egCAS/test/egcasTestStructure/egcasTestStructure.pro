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

INCLUDEPATH += "../../src/"

SOURCES += tst_egcastest_structural.cpp \
           ../../src/structural/egcunaryexpressionnode.cpp \
           ../../src/structural/egcbinaryexpressionnode.cpp \
           ../../src/structural/egcexpressionnode.cpp \
           ../../src/structural/concreteNodes/egcrootexpressionnode.cpp \
           ../../src/structural/concreteNodes/egcnumberexpressionnode.cpp \
           ../../src/structural/egcexpressionnodecreator.cpp \
           ../../src/structural/concreteNodes/egcparenthesisexpressionnode.cpp \
           ../../src/structural/egccontainernode.cpp \
           ../../src/structural/concreteNodes/egcvariableexpressionnode.cpp \
           ../../src/structural/iterator/egcexpressionnodeiterator.cpp \
           ../../src/structural/egcformulaexpression.cpp \
           ../../src/structural/egcbaseexpressionnode.cpp \
           ../../src/structural/egcemptyexpressionnode.cpp \
           ../../src/structural/visitor/egcnodevisitor.cpp \
           ../../src/structural/visitor/egcmaximavisitor.cpp


HEADERS += ../../src/structural/egcunaryexpressionnode.h \
           ../../src/structural/egcbinaryexpressionnode.h \
           ../../src/structural/egcexpressionnode.h \
           ../../src/structural/concreteNodes/egcrootexpressionnode.h \
           ../../src/structural/concreteNodes/egcnumberexpressionnode.h \
           ../../src/structural/egcexpressionnodecreator.h \
           ../../src/structural/concreteNodes/egcvariableexpressionnode.h \
           ../../src/structural/concreteNodes/egcparenthesisexpressionnode.h \
           ../../src/structural/egccontainernode.h \
           tst_egcastest_structural.h \
           ../../src/structural/iterator/egcexpressionnodeiterator.h \
           ../../src/structural/egcformulaexpression.h \
           ../../src/structural/egcbaseexpressionnode.h \
           ../../src/structural/egcemptyexpressionnode.h \
           ../../src/structural/visitor/egcnodevisitor.h \
           ../../src/structural/visitor/egcmaximavisitor.h


DEFINES += SRCDIR=\\\"$$PWD/\\\"
