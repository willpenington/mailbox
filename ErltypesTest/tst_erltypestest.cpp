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

#include <QDebug>

#include "mailboxqt.h"
#include "erlconversion.h"
#include "erlpid.h"

#include "ei.h"
#include "erl_interface.h"

class ErltypesTest : public QObject
{
    Q_OBJECT

public:
    ErltypesTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void conversionToAndFromBuffer_data();
    void conversionToAndFromBuffer();
};

ErltypesTest::ErltypesTest()
{
}

void ErltypesTest::initTestCase()
{
    Mailbox::init();


    ei_x_buff buff;

    int index = buff.index;

    ei_x_new(&buff);
    ei_x_format(&buff, "[1,2,3]");

    buff.index = index;

    int type;
    int size;

    ei_get_type(buff.buff, &(buff.index), &type, &size);

    qDebug() << "type:" << type << "size:" << size;
}

void ErltypesTest::cleanupTestCase()
{
}

void ErltypesTest::conversionToAndFromBuffer_data()
{
    QTest::addColumn<QVariant>("data");
    QTest::addColumn<QVariant>("expected");
    QTest::addColumn<bool>("success");

    // Data
    QVariant ten_s = QVariant::fromValue((short) 10);
    QVariant ten_i = QVariant::fromValue((int) 10);
    QVariant ten_l = QVariant::fromValue((long) 10);

    QTest::newRow("short") << QVariant::fromValue((short) 10) << QVariant(10) << true;
    QTest::newRow("integer") << QVariant::fromValue((int) 10) << QVariant(10) << true;
    QTest::newRow("long") << QVariant::fromValue((long) 10) << QVariant(10) << true;
    QTest::newRow("long_long") << QVariant::fromValue((long long) 10) << QVariant(10) << true;

    QTest::newRow("neg_short") << QVariant::fromValue((short) -10) << QVariant(-10) << true;
    QTest::newRow("neg_integer") << QVariant::fromValue((int) -10) << QVariant(-10) << true;
    QTest::newRow("neg_long") << QVariant::fromValue((long) -10) << QVariant(-10) << true;
    QTest::newRow("neg_long_long") << QVariant::fromValue((long long) -10) << QVariant(-10) << true;

    QTest::newRow("big_short") << QVariant::fromValue((short) SHRT_MAX - 2) << QVariant(SHRT_MAX - 2) << true;
    QTest::newRow("big_int") << QVariant::fromValue((int) INT_MAX - 2) << QVariant(INT_MAX - 2) << true;
    QTest::newRow("big_long") << QVariant::fromValue((long) LONG_MAX - 2) << QVariant::fromValue((long) LONG_MAX - 2) << true;
    QTest::newRow("big_long_long") << QVariant::fromValue((long long) LONG_LONG_MAX - 2) << QVariant::fromValue((long long) LONG_LONG_MAX - 2) << true;

    QTest::newRow("neg_big_short") << QVariant::fromValue((short) SHRT_MIN + 2) << QVariant(SHRT_MIN + 2) << true;
    QTest::newRow("neg_big_int") << QVariant::fromValue((int) INT_MIN + 2) << QVariant(INT_MIN + 2) << true;
    QTest::newRow("neg_big_long") << QVariant::fromValue((long) LONG_MIN + 2) << QVariant::fromValue((long) LONG_MIN + 2) << true;
    QTest::newRow("neg_big_long_long") << QVariant::fromValue((long long) LONG_LONG_MIN + 2) << QVariant::fromValue((long long) LONG_LONG_MIN + 2) << true;

    QTest::newRow("ushort") << QVariant::fromValue((unsigned short) 10) << QVariant(10) << true;
    QTest::newRow("uinteger") << QVariant::fromValue((unsigned int) 10) << QVariant(10) << true;
    QTest::newRow("ulong") << QVariant::fromValue((unsigned long) 10) << QVariant(10) << true;
    QTest::newRow("ulong_long") << QVariant::fromValue((unsigned long long) 10) << QVariant(10) << true;

    QTest::newRow("ubig_short") << QVariant::fromValue((unsigned short) USHRT_MAX - 2) << QVariant(USHRT_MAX - 2) << true;
    QTest::newRow("ubig_int") << QVariant::fromValue((unsigned int) UINT_MAX - 2) << QVariant(UINT_MAX - 2) << true;
    QTest::newRow("ubig_long") << QVariant::fromValue((unsigned long) ULONG_MAX - 2) << QVariant::fromValue((long) ULONG_MAX - 2) << true;
    QTest::newRow("ubig_long") << QVariant::fromValue((unsigned long long) ULONG_LONG_MAX - 2) << QVariant::fromValue((long long) ULONG_LONG_MAX - 2) << true;

    QTest::newRow("float") << QVariant((float) 7.3) << QVariant((float) 7.3) << true;
    QTest::newRow("double") << QVariant((double) 7.3) << QVariant((double) 7.3) << true;

    ETERM *epid = erl_mk_pid("testnode", 1, 2, 3);
    erlang_pid pid;

    ei_x_buff pidbuff;
    ei_x_new(&pidbuff);
    int pidindex = pidbuff.index;
    ei_x_encode_term(&pidbuff, epid);
    ei_decode_pid(pidbuff.buff, &pidindex, &pid);
    ei_x_free(&pidbuff);

    QTest::newRow("pid") << QVariant::fromValue(Mailbox::ErlPid(pid)) << QVariant::fromValue(Mailbox::ErlPid(pid)) << true;
}

void ErltypesTest::conversionToAndFromBuffer()
{
    ei_x_buff buff;

    ei_x_new(&buff);

    int index = buff.index;

    QFETCH(QVariant, data);
    QFETCH(QVariant, expected);

    bool encode_ok = true;
    bool decode_ok = true;

    encode_ok = Mailbox::encode(data, &buff);

    buff.index = index;

    QVariant result = Mailbox::decode(&buff, &decode_ok);

    ei_x_free(&buff);

    QFETCH(bool, success);

    QCOMPARE(encode_ok, success);
    QCOMPARE(decode_ok, success);

    QVERIFY(!success || result == expected);

}

QTEST_APPLESS_MAIN(ErltypesTest)

#include "tst_erltypestest.moc"
