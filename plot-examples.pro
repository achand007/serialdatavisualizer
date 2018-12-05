#
#  QCustomPlot Plot Examples
#

QT       += core gui serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport


CONFIG += c++11

TARGET = plot-examples
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
         ../../qcustomplot.cpp \
    serialportreader.cpp

HEADERS  += mainwindow.h \
         ../../qcustomplot.h \
    serialportreader.h

FORMS    += mainwindow.ui

