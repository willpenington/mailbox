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

#include "erlatom.h"

#include "ei.h"
#include "erl_interface.h"

#include "erlvartypes.h"

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

    void printErlangTerm_data();
    void printErlangTerm();
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

    QVariant pid = Mailbox::build_erl_pid(1, 2, 3, "testnode");
    QTest::newRow("pid") << pid << pid << true;

    QVariant ref = Mailbox::build_erl_ref(3, 1, 2, 3, 1, "testnode");
    QTest::newRow("ref") << ref << ref << true;

    QVariant port = Mailbox::build_erl_port(1, 2, "testnode");
    QTest::newRow("port") << port << port << true;

    QTest::newRow("atom") << QVariant::fromValue(Mailbox::ErlAtom("atomname")) << QVariant::fromValue(Mailbox::ErlAtom("atomname")) << true;

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

void ErltypesTest::printErlangTerm_data()
{
    QTest::addColumn<QVariant>("erlang_value");
    QTest::addColumn<QString>("expected");

    QTest::newRow("int") << QVariant(1) << QString("1");
    QTest::newRow("large_int") << QVariant(123456789) << "123456789";

    QTest::newRow("float") << QVariant(1.1) << QString("1.100000");
    QTest::newRow("big_float") << QVariant(123456.789) << "123456.789000";
    QTest::newRow("round_float") << QVariant(1.0) << "1.000000";

    QTest::newRow("atom") << QVariant::fromValue(Mailbox::ErlAtom("asdf")) << "asdf";

    QVariant pid = Mailbox::build_erl_pid(1, 2, 3, "testnode");

    QTest::newRow("pid") << pid << "<testnode.2.3>";

    QVariant ref = Mailbox::build_erl_ref(3, 1, 2, 3, 1, "testnode");

    QTest::newRow("ref") << ref << "#Ref<1.2.3>";

    QVariant port = Mailbox::build_erl_port(1, 2, "testnode");

    QTest::newRow("port") << port << "#Port<1.2>";
}

void ErltypesTest::printErlangTerm()
{
    QFETCH(QVariant, erlang_value);

    QString result = Mailbox::formatErlangTerm(erlang_value);

    QTEST(result, "expected");

}


QTEST_APPLESS_MAIN(ErltypesTest)

#include "tst_erltypestest.moc"
