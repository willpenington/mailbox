/*
MailSlot
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

#include "erlatom.h"
#include "erlvartypes.h"


#include "mailslotqt.h"
#include "client.h"

class MailSlotTest : public QObject
{
    Q_OBJECT

public:
    MailSlotTest();

private Q_SLOTS:
    void canCommunicateWithErlangShell_data();
    void canCommunicateWithErlangShell();

    void clientCanConnectToErlang_data();
    void clientCanConnectToErlang();

    void canSendMessageToErlang_data();
    void canSendMessageToErlang();

    void canRecieveMessagesFromErlang_data();
    void canRecieveMessagesFromErlang();

    void canSendMultipleMessagesToErlang();
    void canRecieveMultipleMessagesFromErlang();

    void canRoundTripFromCNode_data();
    void canRoundTripFromCNode();

    void unusedNodeDoesNotCrash();

};

QVariantMap simpleValues {
    {"int", 5},
    {"float", 4.3},
    {"round_float", 4.0},
    {"large_int", 1000000},
    {"atom", QVariant::fromValue(MailSlot::ErlAtom("testatom"))}
};

MailSlotTest::MailSlotTest()
{
    MailSlot::init();
}

void MailSlotTest::canCommunicateWithErlangShell_data() {
  QTest::addColumn<QByteArray>("statement");
  QTest::addColumn<QByteArray>("expected");

  QTest::newRow("atom") << QByteArray("sample_atom.") << QByteArray("sample_atom\n");
  QTest::newRow("maths") << QByteArray("1 + 1.") << QByteArray("2\n");
  QTest::newRow("complex types") << QByteArray("[{foo, bar}, 1 + 3, baz].")
                                 << QByteArray("[{foo,bar},4,baz]\n");
}

void MailSlotTest::canCommunicateWithErlangShell()
{
  ErlangShell erl("dummy", "cookie");

  QFETCH(QByteArray, statement);
  QTEST(erl.execStatement(statement), "expected");
}

void MailSlotTest::clientCanConnectToErlang_data()
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

void MailSlotTest::clientCanConnectToErlang()
{
  QFETCH(QByteArray, erlangNodeName);
  QFETCH(QByteArray, cnodeNodeName);
  QFETCH(QByteArray, cnodeConnectTo);
  QFETCH(QByteArray, erlangCookie);
  QFETCH(QByteArray, cnodeCookie);

  ErlangShell erl(erlangNodeName, erlangCookie);

  MailSlot::Client *node = new MailSlot::Client();

  QTEST(node->connect(cnodeNodeName, cnodeConnectTo, cnodeCookie), "expected");

  delete(node);
}

void MailSlotTest::canSendMessageToErlang_data()
{
//    QTest::addColumn<QVariant>("value");

//    QTest::newRow("int") <<  QVariant(1);

//    MailSlot::ErlAtom atom("testatom");

//    QTest::newRow("atom") << QVariant::fromValue(atom);



    QTest::addColumn<QVariant>("value");

    QVariantMap::const_iterator i = simpleValues.constBegin();

    while (i != simpleValues.constEnd()) {
        QTest::newRow(i.key().toLocal8Bit().data()) << i.value();
        i++;
    }
}

QString erl_output(QVariant value)
{
    QString raw = MailSlot::formatErlangTerm(value);

    QRegularExpression re;
    re.setPattern("(\\d+\\.\\d*?[1-9])0+");
    QRegularExpressionMatch match =  re.match(raw);
    if (match.hasMatch()) {
        QString result = match.captured(1);
        return result;
    }

    re.setPattern("(\\d+\\.0)0+");
    match =  re.match(raw);

    if (match.hasMatch()) {
        QString result = match.captured(1);
        return result;
    }

    return raw;
}

void MailSlotTest::canSendMessageToErlang()
{
  QFETCH(QVariant, value);

  ErlangShell erl("sendmessage", "cookie");
  MailSlot::Client *node = new MailSlot::Client();
  QVERIFY(node->connect("sendmessagelib", "sendmessage", "cookie"));

  erl.execStatement("register(shell, self()).");

  QCOMPARE(erl.execStatement("flush()."), QByteArray("ok\n"));

  node->sendMessage("shell", value);


  QByteArray full_result = QByteArray("Shell got ") + erl_output(value).toLatin1() + QByteArray("\n");
  
  QCOMPARE(erl.execStatement("flush()."), full_result);

  delete(node);
}

void MailSlotTest::canRecieveMessagesFromErlang_data()
{
    QTest::addColumn<QVariant>("value");

    QVariantMap::const_iterator i = simpleValues.constBegin();

    while (i != simpleValues.constEnd()) {
        QTest::newRow(i.key().toLocal8Bit().data()) << i.value();
        i++;
    }

}

void MailSlotTest::canRecieveMessagesFromErlang()
{

    ErlangShell erl("recvmessage", "cookie");
    MailSlot::Client *node = new MailSlot::Client();
    QVERIFY(node->connect("recvmessagelib", "recvmessage", "cookie"));

    QSignalSpy recvSpy(node, &MailSlot::Client::messageRecieved);

    erl.execStatement("register(shell, self()).");

    QVariant self = node->self();

    node->sendMessage("shell", self);

    QCOMPARE(recvSpy.count(), 0);

    QFETCH(QVariant, value);
    QByteArray val_str = MailSlot::formatErlangTerm(value).toUtf8();

    QString stmt = "receive\n Pid -> Pid ! " + val_str + "\n end.";

    erl.execStatement(stmt.toUtf8());

    QVERIFY(recvSpy.count() == 1 || recvSpy.wait(1000));

    QVariant to = recvSpy.first()[0];
    QVariant contents = recvSpy.first()[1];

    // Check message destination
    QCOMPARE(to, self);

    // Check message contents
    QCOMPARE(contents, value);

    delete(node);
}

void MailSlotTest::canSendMultipleMessagesToErlang()
{
  ErlangShell erl("sendmultimessage", "cookie");
  MailSlot::Client *node = new MailSlot::Client();
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

void MailSlotTest::canRecieveMultipleMessagesFromErlang()
{

    ErlangShell erl("rmm", "cookie");
    MailSlot::Client *node = new MailSlot::Client();
    QVERIFY(node->connect("rmml", "rmm", "cookie"));

    QSignalSpy recvSpy(node, &MailSlot::Client::messageRecieved);

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

void MailSlotTest::canRoundTripFromCNode_data()
{

    QTest::addColumn<QVariant>("value");

    QVariantMap::const_iterator i = simpleValues.constBegin();

    while (i != simpleValues.constEnd()) {
        QTest::newRow(i.key().toLocal8Bit().data()) << i.value();
        i++;
    }
}

void MailSlotTest::canRoundTripFromCNode()
{
    QFETCH(QVariant, value);

    ErlangShell erl("roundtrip", "cookie");
    MailSlot::Client *node = new MailSlot::Client();
    QVERIFY(node->connect("roundtriplib", "roundtrip", "cookie"));

    QSignalSpy recvSpy(node, &MailSlot::Client::messageRecieved);

    erl.execStatement("register(shell, self()).");
    node->sendPid("shell");

    QCOMPARE(recvSpy.count(), 0);

    erl.execStatement("Pid = receive\n  Pid -> Pid\n end.");
    node->sendMessage("shell", value);
    erl.execStatement("receive\n Val -> Pid ! Val\n end.");

    QVERIFY(recvSpy.count() == 1 || recvSpy.wait(1000));
    QCOMPARE(recvSpy.takeFirst()[1], value);
}

void MailSlotTest::unusedNodeDoesNotCrash()
{
    MailSlot::Client *node = new MailSlot::Client();
    delete(node);
}

QTEST_GUILESS_MAIN(MailSlotTest)

#include "tst_mailslottest.moc"
