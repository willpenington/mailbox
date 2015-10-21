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

CONFIG += c++11

TEMPLATE = app


SOURCES += tst_mailboxtest.cpp \
    erlangshell.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    erlangshell.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MailboxQt/release/ -lMailboxQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MailboxQt/debug/ -lMailboxQt
else:unix: LIBS += -L$$OUT_PWD/../MailboxQt/ -lMailboxQt

INCLUDEPATH += $$PWD/../MailboxQt
DEPENDPATH += $$PWD/../MailboxQt

#PRE_TARGETDEPS += $$PWD/../MailboxQt/libMailboxQt.so
