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
#include <QSignalSpy>

#include "erlangshell.h"

#include "mailboxqt.h"
#include "client.h"

class MailboxTest : public QObject
{
    Q_OBJECT

public:
    MailboxTest();

private Q_SLOTS:
    void canCommunicateWithErlangShell_data();
    void canCommunicateWithErlangShell();

    void clientCanConnectToErlang_data();
    void clientCanConnectToErlang();

    void canSendMessageToErlang();
    void canRecieveMessagesFromErlang();

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
  ErlangShell erl("dummy", "cookie");

  QFETCH(QByteArray, statement);
  QTEST(erl.execStatement(statement), "expected");
}

void MailboxTest::clientCanConnectToErlang_data()
{
  QTest::addColumn<QByteArray>("erlangNodeName");
  QTest::addColumn<QByteArray>("cnodeNodeName");
  QTest::addColumn<QByteArray>("cnodeConnectTo");
  QTest::addColumn<QByteArray>("erlangCookie");
  QTest::addColumn<QByteArray>("cnodeCookie");
  QTest::addColumn<bool>("expected");

  QTest::newRow("normal") << QByteArray("conTest1") << QByteArray("conTest1Lib")
                          << QByteArray("conTest1") << QByteArray("cookie")
                          << QByteArray("cookie") << true;
  QTest::newRow("all_different") << QByteArray("conTest2")
                                 << QByteArray("conTest2Lib")
                                 << QByteArray("wrong") << QByteArray("cookie1")
                                 << QByteArray("cookie2") << false;
  QTest::newRow("names_different") << QByteArray("conTest2")
                                 << QByteArray("conTest2Lib")
                                 << QByteArray("wrong") << QByteArray("cookie")
                                 << QByteArray("cookie") << false;
  QTest::newRow("cookies_different") << QByteArray("conTest1")
                          << QByteArray("conTest1Lib") << QByteArray("conTest1")
                          << QByteArray("cookie1") << QByteArray("cookie2")
                          << false;
  QTest::newRow("all_same") << QByteArray("conTest3") << QByteArray("conTest3")
                            << QByteArray("conTest3") << QByteArray("cookie")
                            << QByteArray("cookie") << false;
}

void MailboxTest::clientCanConnectToErlang()
{
  QFETCH(QByteArray, erlangNodeName);
  QFETCH(QByteArray, cnodeNodeName);
  QFETCH(QByteArray, cnodeConnectTo);
  QFETCH(QByteArray, erlangCookie);
  QFETCH(QByteArray, cnodeCookie);

  ErlangShell erl(erlangNodeName, erlangCookie);

  Mailbox::Client *node = new Mailbox::Client();

  QTEST(node->connect(cnodeNodeName, cnodeConnectTo, cnodeCookie), "expected");

  delete(node);
}

void MailboxTest::canSendMessageToErlang()
{
  ErlangShell erl("sendmessage", "cookie");
  Mailbox::Client *node = new Mailbox::Client();
  QVERIFY(node->connect("sendmessagelib", "sendmessage", "cookie"));

  erl.execStatement("register(shell, self()).");

  node->sendAtom("shell", "testmessage");
  
  QCOMPARE(erl.execStatement("flush()."), QByteArray("Shell got testmessage\n"));

  delete(node);
}

void MailboxTest::canRecieveMessagesFromErlang()
{
    QVERIFY(false);
    return;

    ErlangShell erl("recvmessage", "cookie");
    Mailbox::Client *node = new Mailbox::Client();
    QVERIFY(node->connect("recvmessage", "recvmessagelib", "cookie"));

    QSignalSpy recvSpy(node, SIGNAL(messageRecieved));

    erl.execStatement("{sendmessagelib, foo} ! bar.");

    QVERIFY(recvSpy.wait(1));
    QCOMPARE(recvSpy.count(), 1);

    delete(node);
}

QTEST_APPLESS_MAIN(MailboxTest)

#include "tst_mailboxtest.moc"
