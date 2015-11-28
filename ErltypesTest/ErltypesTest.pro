#-------------------------------------------------
#
# Project created by QtCreator 2015-10-20T14:45:22
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_erltypestest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_erltypestest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MailSlotQt/release/ -lMailSlotQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MailSlotQt/debug/ -lMailSlotQt
else:unix: LIBS += -L$$OUT_PWD/../MailSlotQt/ -lMailSlotQt

INCLUDEPATH += $$PWD/../MailSlotQt
DEPENDPATH += $$PWD/../MailSlotQt

OTPPATH = /usr/lib/erlang

unix|win32: LIBS += -L$$OTPPATH/usr/lib/erlang/usr/lib/ -lerl_interface
unix|win32: LIBS += -L$$OTPPATH/usr/lib/ -lei

INCLUDEPATH += $$OTPPATH/usr/include
DEPENDPATH += $$OTPPATH/usr/include

win32:!win32-g++: PRE_TARGETDEPS += $$OTPPATH/usr/lib/erl_interface.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OTPPATH/usr/lib/liberl_interface.a


win32:!win32-g++: PRE_TARGETDEPS += $$OTPPATH/usr/lib/ei.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OTPPATH/usr/lib/libei.a

HEADERS +=

