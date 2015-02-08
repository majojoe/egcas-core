#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T19:54:08
#
#-------------------------------------------------

QT       += core gui \
            printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = egCAS
TEMPLATE = app

CONFIG += c++11

LIBS += -legcas

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/view/egcasscene.cpp \
    src/view/egcasiteminterface.cpp \
    src/view/egctextitem.cpp \
    src/view/egcpixmapitem.cpp \
    src/view/resizehandle.cpp \
    src/view/egcformulaitem.cpp \
    src/structural/egcexpressionnode.cpp \
    src/structural/egcbinaryexpressionnode.cpp \
    src/structural/egcunaryexpressionnode.cpp \
    src/structural/egcexpressionnodecreator.cpp \
    src/structural/visitor/egcnodevisitor.cpp \
    src/structural/egccontainernode.cpp \
    src/structural/egcformulaexpression.cpp \
    src/structural/iterator/egcexpressionnodeiterator.cpp \
    src/structural/egcbaseexpressionnode.cpp \
    src/structural/egcemptyexpressionnode.cpp \
    src/structural/visitor/egcmaximavisitor.cpp \
    src/structural/concreteNodes/egcnumberexpressionnode.cpp \
    src/structural/concreteNodes/egcparenthesisexpressionnode.cpp \
    src/structural/concreteNodes/egcrootexpressionnode.cpp \
    src/structural/concreteNodes/egcvariableexpressionnode.cpp \
    src/structural/visitor/egcmathmlvisitor.cpp \
    src/casKernel/egcmaximaconn.cpp \
    src/casKernel/egckernelconn.cpp

HEADERS  += src/mainwindow.h \
    src/view/egcasscene.h \
    src/view/egcasiteminterface.h \
    src/view/egctextitem.h \
    src/view/egcpixmapitem.h \
    src/view/resizehandle.h \
    src/view/egcformulaitem.h \
    src/structural/egcexpressionnode.h \
    src/structural/egcbinaryexpressionnode.h \
    src/structural/egcunaryexpressionnode.h \
    src/structural/egcexpressionnodecreator.h \
    src/structural/visitor/egcnodevisitor.h \
    src/structural/egccontainernode.h \
    src/structural/egcformulaexpression.h \
    src/structural/iterator/egcexpressionnodeiterator.h \
    src/structural/egcnodes.h \
    src/structural/egcbaseexpressionnode.h \
    src/structural/egcemptyexpressionnode.h \
    src/structural/visitor/egcmaximavisitor.h \
    src/structural/concreteNodes/egcnumberexpressionnode.h \
    src/structural/concreteNodes/egcparenthesisexpressionnode.h \
    src/structural/concreteNodes/egcrootexpressionnode.h \
    src/structural/concreteNodes/egcvariableexpressionnode.h \
    src/structural/visitor/egcmathmlvisitor.h \
    src/casKernel/egcmaximaconn.h \
    src/casKernel/egckernelconn.h

FORMS    += src/ui/mainwindow.ui

RESOURCES += \
    img/ressources.qrc
