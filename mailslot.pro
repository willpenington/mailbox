TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    MailSlotTest \
    MailSlotQt \
    ErltypesTest

MailSlotTest.depends = MailSlotQt
