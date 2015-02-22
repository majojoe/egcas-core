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
           ../../src/structural/specialNodes/egcunaryexpressionnode.cpp \
           ../../src/structural/specialNodes/egcbinaryexpressionnode.cpp \
           ../../src/structural/specialNodes/egcexpressionnode.cpp \
           ../../src/structural/concreteNodes/egcrootexpressionnode.cpp \
           ../../src/structural/concreteNodes/egcnumberexpressionnode.cpp \
           ../../src/structural/egcexpressionnodecreator.cpp \
           ../../src/structural/concreteNodes/egcparenthesisexpressionnode.cpp \
           ../../src/structural/specialNodes/egccontainernode.cpp \
           ../../src/structural/concreteNodes/egcvariableexpressionnode.cpp \
           ../../src/structural/iterator/egcexpressionnodeiterator.cpp \
           ../../src/structural/egcformulaexpression.cpp \
           ../../src/structural/specialNodes/egcbaseexpressionnode.cpp \
           ../../src/structural/specialNodes/egcemptyexpressionnode.cpp \
           ../../src/structural/visitor/egcnodevisitor.cpp \
           ../../src/structural/visitor/egcmaximavisitor.cpp \
           ../../src/structural/visitor/egcmathmlvisitor.cpp

HEADERS += ../../src/structural/specialNodes/egcunaryexpressionnode.h \
           ../../src/structural/specialNodes/egcbinaryexpressionnode.h \
           ../../src/structural/specialNodes/egcexpressionnode.h \
           ../../src/structural/concreteNodes/egcrootexpressionnode.h \
           ../../src/structural/concreteNodes/egcnumberexpressionnode.h \
           ../../src/structural/egcexpressionnodecreator.h \
           ../../src/structural/concreteNodes/egcvariableexpressionnode.h \
           ../../src/structural/concreteNodes/egcparenthesisexpressionnode.h \
           ../../src/structural/specialNodes/egccontainernode.h \
           tst_egcastest_structural.h \
           ../../src/structural/iterator/egcexpressionnodeiterator.h \
           ../../src/structural/egcformulaexpression.h \
           ../../src/structural/specialNodes/egcbaseexpressionnode.h \
           ../../src/structural/specialNodes/egcemptyexpressionnode.h \
           ../../src/structural/visitor/egcnodevisitor.h \
           ../../src/structural/visitor/egcmaximavisitor.h \
           ../../src/structural/visitor/egcmathmlvisitor.h


DEFINES += SRCDIR=\\\"$$PWD/\\\"
