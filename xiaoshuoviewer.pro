#-------------------------------------------------
#
# Project created by QtCreator 2013-05-22T18:25:06
#
#-------------------------------------------------

QT       += core gui
QT       += webkit network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xiaoshuoviewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myqwebpage.cpp

HEADERS  += mainwindow.h \
    myqwebpage.h

FORMS    += mainwindow.ui
