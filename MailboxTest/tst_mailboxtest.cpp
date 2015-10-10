/*
Mailbox
Copyright (C) 2015 Will Penington

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
USA
*/


#include <QString>
#include <QtTest>

#include "erlangshell.h"
#include "mailboxqt.h"

class MailboxTest : public QObject
{
    Q_OBJECT

public:
    MailboxTest();

private Q_SLOTS:
    void canCommunicateWithErlangShell_data();
    void canCommunicateWithErlangShell();
};

MailboxTest::MailboxTest()
{
    Mailbox::init();
}

void MailboxTest::canCommunicateWithErlangShell_data() {
  QTest::addColumn<QByteArray>("statement");
  QTest::addColumn<QByteArray>("expected");

  QTest::newRow("atom") << QByteArray("sample_atom.") << QByteArray("sample_atom\n");
  QTest::newRow("maths") << QByteArray("1 + 1.") << QByteArray("2\n");
  QTest::newRow("complex types") << QByteArray("[{foo, bar}, 1 + 3, baz].")
                                 << QByteArray("[{foo,bar},4,baz]\n");
}

void MailboxTest::canCommunicateWithErlangShell()
{
  ErlangShell erl("dummy");

  QFETCH(QByteArray, statement);
  QTEST(erl.execStatement(statement), "expected");
}




QTEST_APPLESS_MAIN(MailboxTest)

#include "tst_mailboxtest.moc"
