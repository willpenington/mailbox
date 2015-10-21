TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    MailboxTest \
    MailboxQt \
    ErltypesTest

MailboxTest.depends = MailboxQt
