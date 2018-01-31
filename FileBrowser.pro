#-------------------------------------------------
#
# Project created by QtCreator 2018-01-28T19:13:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileBrowser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    customtableview.cpp \
    configxmlreader.cpp \
    filesystementry.cpp \
    customfilesystemmodel.cpp

HEADERS  += mainwindow.h \
    customtableview.h \
    configxmlreader.h \
    filesystementry.h \
    customfilesystemmodel.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
