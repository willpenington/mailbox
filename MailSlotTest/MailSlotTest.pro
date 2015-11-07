#-------------------------------------------------
#
# Project created by QtCreator 2015-10-03T11:33:26
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_mailslottest
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += c++11

TEMPLATE = app


SOURCES += tst_mailslottest.cpp \
    erlangshell.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    erlangshell.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MailSlotQt/release/ -lMailSlotQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MailSlotQt/debug/ -lMailSlotQt
else:unix: LIBS += -L$$OUT_PWD/../MailSlotQt/ -lMailSlotQt

INCLUDEPATH += $$PWD/../MailSlotQt
DEPENDPATH += $$PWD/../MailSlotQt

#PRE_TARGETDEPS += $$PWD/../MailSlotQt/libMailSlotQt.so
