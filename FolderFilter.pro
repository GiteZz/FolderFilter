#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T14:51:37
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FolderFilter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    handlesettings.cpp

HEADERS  += mainwindow.h \
    handlesettings.h \
    structs.h \
    enums.h

FORMS    += mainwindow.ui
