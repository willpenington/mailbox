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

#include "mailboxqt.h"
#include "erlconversion.h"

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
    QTest::newRow("integer") << QVariant(10) << QVariant(10) << true;
}

void ErltypesTest::conversionToAndFromBuffer()
{
    ei_x_buff buff;

    ei_x_new_with_version(&buff);

    int index = buff.index;

    QFETCH(QVariant, data);
    QFETCH(QVariant, expected);

    bool encode_ok;
    bool decode_ok;

    encode_ok = Mailbox::encode(data, &buff);

    buff.index = index;

    QVariant result = Mailbox::decode(&buff, &decode_ok);

    QFETCH(bool, success);

    QCOMPARE(encode_ok, success);
    QCOMPARE(decode_ok, success);

    if (success)
        QCOMPARE(result, expected);

}

QTEST_APPLESS_MAIN(ErltypesTest)

#include "tst_erltypestest.moc"
