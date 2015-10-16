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

    void canSendMultipleMessagesToErlang();
    void canRecieveMultipleMessagesFromErlang();

    void unusedNodeDoesNotCrash();

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

  QCOMPARE(erl.execStatement("flush()."), QByteArray("ok\n"));

  node->sendAtom("shell", "testmessage");
  
  QCOMPARE(erl.execStatement("flush()."), QByteArray("Shell got testmessage\n"));

  delete(node);
}

void MailboxTest::canRecieveMessagesFromErlang()
{

    ErlangShell erl("recvmessage", "cookie");
    Mailbox::Client *node = new Mailbox::Client();
    QVERIFY(node->connect("recvmessagelib", "recvmessage", "cookie"));

    QSignalSpy recvSpy(node, SIGNAL(messageRecieved()));

    erl.execStatement("register(shell, self()).");
    node->sendPid("shell");

    QCOMPARE(recvSpy.count(), 0);

    erl.execStatement("receive\n  Pid -> Pid ! hello \n end.");

    QVERIFY(recvSpy.count() == 1 || recvSpy.wait(1000));

    delete(node);
}

void MailboxTest::canSendMultipleMessagesToErlang()
{
  ErlangShell erl("sendmultimessage", "cookie");
  Mailbox::Client *node = new Mailbox::Client();
  QVERIFY(node->connect("sendmultimessagelib", "sendmultimessage", "cookie"));

  erl.execStatement("register(shell, self()).");

  QCOMPARE(erl.execStatement("flush()."), QByteArray("ok\n"));

  node->sendAtom("shell", "test1");

  QThread::sleep(1);

  QCOMPARE(erl.execStatement("flush()."), QByteArray("Shell got test1\n"));

  node->sendAtom("shell", "test2");

  QThread::sleep(1);

  QCOMPARE(erl.execStatement("flush()."), QByteArray("Shell got test2\n"));

  node->sendAtom("shell", "test3");

  QThread::sleep(1);

  QCOMPARE(erl.execStatement("flush()."), QByteArray("Shell got test3\n"));

  delete(node);
}

void MailboxTest::canRecieveMultipleMessagesFromErlang()
{

    ErlangShell erl("rmm", "cookie");
    Mailbox::Client *node = new Mailbox::Client();
    QVERIFY(node->connect("rmml", "rmm", "cookie"));

    QSignalSpy recvSpy(node, SIGNAL(messageRecieved()));

    erl.execStatement("register(shell, self()).");
    node->sendPid("shell");

    QCOMPARE(recvSpy.count(), 0);

    erl.execStatement("Pid = receive\n  Pid -> Pid\n end.");
    erl.execStatement("Pid ! hello.");

    QVERIFY(recvSpy.count() == 1 || recvSpy.wait(1000));

    node->sendPid("shell");
    erl.execStatement("Pid ! hello.");
    QVERIFY(recvSpy.count() == 2 || recvSpy.wait(1000));

    node->sendPid("shell");
    erl.execStatement("Pid ! hello.");
    QVERIFY(recvSpy.count() == 3 || recvSpy.wait(1000));

    node->sendPid("shell");
    erl.execStatement("Pid ! hello.");
    QVERIFY(recvSpy.count() == 4 || recvSpy.wait(1000));

    delete(node);
}

void MailboxTest::unusedNodeDoesNotCrash()
{
    Mailbox::Client *node = new Mailbox::Client();
    delete(node);
}

QTEST_GUILESS_MAIN(MailboxTest)

#include "tst_mailboxtest.moc"
