TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    MailboxTest \
    MailboxQt

MailboxTest.depends = MailboxQt
