#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T22:44:20
#
#-------------------------------------------------

QT       -= gui

CONFIG += c++11

TARGET = MailSlotQt
TEMPLATE = lib

DEFINES += MAILSLOTQT_LIBRARY

SOURCES += mailslotqt.cpp \
    client.cpp \
    msglistener.cpp \
    erlconversion.cpp \
    erlpid.cpp \
    erlref.cpp \
    erlport.cpp \
    erlatom.cpp \
    erlvartypes.cpp \
    process.cpp

HEADERS += mailslotqt.h\
        mailslotqt_global.h \
    client.h \
    msglistener.h \
    erlconversion.h \
    erlpid.h \
    erlref.h \
    erlport.h \
    erlatom.h \
    erlvartypes.h \
    process.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTPPATH = /usr/lib/erlang

unix|win32: LIBS += -L$$OTPPATH/usr/lib/erlang/usr/lib/ -lerl_interface
unix|win32: LIBS += -L$$OTPPATH/usr/lib/ -lei

INCLUDEPATH += $$OTPPATH/usr/include
DEPENDPATH += $$OTPPATH/usr/include

win32:!win32-g++: PRE_TARGETDEPS += $$OTPPATH/usr/lib/erl_interface.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OTPPATH/usr/lib/liberl_interface.a


win32:!win32-g++: PRE_TARGETDEPS += $$OTPPATH/usr/lib/ei.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OTPPATH/usr/lib/libei.a


