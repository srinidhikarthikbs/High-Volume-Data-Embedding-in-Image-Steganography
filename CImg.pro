#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T09:30:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CImg
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    embedding.cpp \
    extraction.cpp

HEADERS  += mainwindow.h \
    ../Downloads/CImg-1.6.6_pre093015/CImg.h \
    embedding.h \
    extraction.h

FORMS    += mainwindow.ui \
    embedding.ui \
    extraction.ui

QMAKE_CXXFLAGS += -fopenmp -lgd
LIBS += -fopenmp -lgd
