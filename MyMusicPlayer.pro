#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T14:24:05
#
#-------------------------------------------------

QT       += core gui
QT      += widgets
QT      += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMusicPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    widget.cpp

HEADERS  += mainwindow.h \
    widget.h

FORMS    += mainwindow.ui \
    widget.ui

RESOURCES += \
    songs.qrc \
    icons.qrc
