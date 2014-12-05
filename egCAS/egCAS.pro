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
    src/structural/egcrootexpressionnode.cpp \
    src/structural/egcnumberexpressionnode.cpp \
    src/structural/egcvariableexpressionnode.cpp \
    src/structural/egcexpressionnodecreator.cpp \
    src/structural/egcparenthesisexpressionnode.cpp \
    src/structural/visitor/egcnodevisitor.cpp \
    src/structural/egccontainernode.cpp \
    src/structural/egcformulaexpression.cpp \
    src/structural/iterator/egcexpressionnodeiterator.cpp \
    src/structural/iterator/egcstepnodeiterator.cpp \
    src/structural/egcbaseexpressionnode.cpp \
    src/structural/egcemptyexpressionnode.cpp

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
    src/structural/egcrootexpressionnode.h \
    src/structural/egcnumberexpressionnode.h \
    src/structural/egcvariableexpressionnode.h \
    src/structural/egcexpressionnodecreator.h \
    src/structural/egcparenthesisexpressionnode.h \
    src/structural/visitor/egcnodevisitor.h \
    src/structural/egccontainernode.h \
    src/structural/egcformulaexpression.h \
    src/structural/iterator/egcexpressionnodeiterator.h \
    src/structural/egcnodes.h \
    src/structural/iterator/egcstepnodeiterator.h \
    src/structural/egcbaseexpressionnode.h \
    src/structural/egcemptyexpressionnode.h

FORMS    += src/ui/mainwindow.ui

RESOURCES += \
    img/ressources.qrc
