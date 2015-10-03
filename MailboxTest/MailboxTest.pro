#-------------------------------------------------
#
# Project created by QtCreator 2015-10-03T11:33:26
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_mailboxtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_mailboxtest.cpp \
    erlangshell.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    erlangshell.h
