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

#include "erlconversion.h"

#include <QDebug>

#include <erlpid.h>
#include "erlref.h"
#include "erlport.h"
#include "erlatom.h"

#include "mailslotqt.h"

namespace MailSlot {
bool encodeDouble(QVariant var, ei_x_buff *buff);
QVariant decodeDouble(ei_x_buff *buff, bool *ok);

bool encodeChar(QVariant var, ei_x_buff *buff);
bool encodeLong(QVariant var, ei_x_buff *buff);
bool encodeULong(QVariant var, ei_x_buff *buff);
bool encodeLongLong(QVariant var, ei_x_buff *buff);
bool encodeULongLong(QVariant var, ei_x_buff *buff);

QVariant decodeInt(ei_x_buff *buff, bool *ok);

bool encodePid(QVariant var, ei_x_buff *buff);
QVariant decodePid(ei_x_buff *buff, bool *ok);

bool encodeRef(QVariant var, ei_x_buff *buff);
QVariant decodeRef(ei_x_buff *buff, bool *ok);

bool encodePort(QVariant var, ei_x_buff *buff);
QVariant decodePort(ei_x_buff *buff, bool *ok);

bool encodeAtom(QVariant var, ei_x_buff *buff);
QVariant decodeAtom(ei_x_buff *buff, bool *ok);

QVariant decode(ei_x_buff *buff, bool *ok)
{
    int type = 0;
    int size = 0;

    ei_get_type(buff->buff, &(buff->index), &type, &size);

    switch(type) {
    case ERL_FLOAT_EXT:
        return decodeDouble(buff, ok);
        break;
    case ERL_INTEGER_EXT:
    case ERL_SMALL_INTEGER_EXT:
    case ERL_SMALL_BIG_EXT:
    case ERL_LARGE_BIG_EXT:
        return decodeInt(buff, ok);
    case ERL_PID_EXT:
        return decodePid(buff, ok);
    case ERL_NEW_REFERENCE_EXT:
    case ERL_REFERENCE_EXT:
        return decodeRef(buff, ok);
    case ERL_PORT_EXT:
        return decodePort(buff, ok);
    case ERL_ATOM_EXT:
    case ERL_ATOM_UTF8_EXT:
    case ERL_SMALL_ATOM_UTF8_EXT:
    case ERL_SMALL_ATOM_EXT:
        return decodeAtom(buff, ok);
    default:
        qDebug() << type;
        qDebug() << size;
        break;
    }

    if (ok != nullptr)
        *ok = false;

    return QVariant();
}

bool encode(QVariant var, ei_x_buff *buff)
{
    switch (var.type()) {
    case QMetaType::Float:
    case QMetaType::Double:
        return encodeDouble(var, buff);
        break;
    case QMetaType::Char:
        return encodeChar(var, buff);
    case QMetaType::UShort:
    case QMetaType::UInt:
    case QMetaType::ULong:
        return encodeULong(var, buff);
    case QMetaType::Short:
    case QMetaType::Int:
    case QMetaType::Long:
        return encodeLong(var, buff);
    case QMetaType::LongLong:
        return encodeLongLong(var, buff);
    case QMetaType::ULongLong:
        return encodeULongLong(var, buff);
    default:
        break;
    }

    if (var.userType() == pidMetaType)
        return encodePid(var, buff);
    else if (var.userType() == refMetaType)
        return encodeRef(var, buff);
    else if (var.userType() == portMetaType)
        return encodePort(var, buff);
    else if (var.userType() == atomMetaType)
        return encodeAtom(var, buff);

    return false;
}

QString formatErlangTerm(QVariant var)
{
    ei_x_buff buff;
    ei_x_new(&buff);
    int index = buff.index;

    encode(var, &buff);

    char result[MAXATOMLEN_UTF8];
    char *result_ptr = result;

    ei_s_print_term(&result_ptr, buff.buff, &index);

    QString str(result);
    return str;

    return QString(result);

}

/* Type Specific Implementations */
QVariant decodeDouble(ei_x_buff *buff, bool *ok)
{
    double val;

    if (ei_decode_double(buff->buff, &(buff->index), &val) != 0) {
        *ok = false;
        return QVariant();
    }

    return QVariant(val);
}

bool encodeDouble(QVariant var, ei_x_buff *buff)
{
    bool convert_ok = true;
    double val = var.toDouble(&convert_ok);

    if (!convert_ok)
        return false;

    return (ei_x_encode_double(buff, val) == 0);
}

QVariant decodeInt(ei_x_buff *buff, bool *ok)
{
    char char_val;

    long long_val;
    unsigned long ulong_val;

    long long long_long_val;
    unsigned long long ulong_long_val;

    if (ei_decode_char(buff->buff, &(buff->index), &char_val) == 0) {
        *ok = true;
        return QVariant::fromValue(static_cast<unsigned char>(char_val));
    }

    if (ei_decode_long(buff->buff, &(buff->index), &long_val) == 0) {
        *ok = true;
        return QVariant::fromValue(long_val);
    }

    if (ei_decode_ulong(buff->buff, &(buff->index), &ulong_val) == 0) {
        *ok = true;
        return QVariant::fromValue(ulong_val);
    }

    if (ei_decode_longlong(buff->buff, &(buff->index), &long_long_val) == 0) {
        *ok = true;
        return QVariant::fromValue(long_val);
    }

    if (ei_decode_ulonglong(buff->buff, &(buff->index), &ulong_long_val) == 0) {
        *ok = true;
        return QVariant::fromValue(ulong_val);
    }

    *ok = false;
    return QVariant();

}

bool encodeLong(QVariant var, ei_x_buff *buff)
{
    bool convert_ok = true;
    long val = qvariant_cast<long>(var);

    if (!convert_ok)
        return false;

    bool encode_ok = true;
    encode_ok = (ei_x_encode_long(buff, val) == 0);

    return encode_ok;
}

bool encodeLongLong(QVariant var, ei_x_buff *buff)
{
    bool convert_ok = true;
    long val = qvariant_cast<long long>(var);

    if (!convert_ok)
        return false;

    bool encode_ok = true;
    encode_ok = (ei_x_encode_longlong(buff, val) == 0);

    return encode_ok;
}

bool encodeULong(QVariant var, ei_x_buff *buff)
{
    bool convert_ok = true;
    unsigned long val = qvariant_cast<unsigned long>(var);

    if (!convert_ok)
        return false;

    bool encode_ok = true;
    encode_ok = (ei_x_encode_ulong(buff, val) == 0);

    return encode_ok;
}

bool encodeULongLong(QVariant var, ei_x_buff *buff)
{
    bool convert_ok = true;
    unsigned long val = qvariant_cast<unsigned long long>(var);

    if (!convert_ok)
        return false;

    bool encode_ok = true;
    encode_ok = (ei_x_encode_ulonglong(buff, val) == 0);

    return encode_ok;
}

bool encodeChar(QVariant var, ei_x_buff *buff)
{
    bool convert_ok = true;
    char val = qvariant_cast<char>(var);

    if (!convert_ok)
        return false;

    bool encode_ok = true;
    encode_ok = (ei_x_encode_char(buff, val) == 0);

    return encode_ok;
}

QVariant decodePid(ei_x_buff *buff, bool *ok)
{
    erlang_pid val;

    if (ei_decode_pid(buff->buff, &(buff->index), &val) != 0) {
        *ok = false;
        return QVariant();
    }

    return QVariant::fromValue(ErlPid(&val));
}

bool encodePid(QVariant var, ei_x_buff *buff)
{
    ErlPid val = qvariant_cast<ErlPid>(var);

    erlang_pid pid =  *((erlang_pid *) val.raw_pid());

    bool encode_ok;
    encode_ok = ei_x_encode_pid(buff, &pid) == 0;

    return encode_ok;
}

QVariant decodeRef(ei_x_buff *buff, bool *ok)
{
    erlang_ref val;

    if (ei_decode_ref(buff->buff, &(buff->index), &val) != 0) {
        *ok = false;
        return QVariant();
    }

    return QVariant::fromValue(ErlRef(&val));
}

bool encodeRef(QVariant var, ei_x_buff *buff)
{
    ErlRef val = qvariant_cast<ErlRef>(var);

    erlang_ref ref = *((erlang_ref *) val.raw_ref());

    bool encode_ok;
    encode_ok = ei_x_encode_ref(buff, &ref) == 0;

    return encode_ok;
}

QVariant decodePort(ei_x_buff *buff, bool *ok)
{
    erlang_port val;

    if (ei_decode_port(buff->buff, &(buff->index), &val) != 0) {
        *ok = false;
        return QVariant();
    }

    return QVariant::fromValue(ErlPort(&val));
}

bool encodePort(QVariant var, ei_x_buff *buff)
{
    ErlPort port = qvariant_cast<ErlPort>(var);

    return ei_x_encode_port(buff, (erlang_port *) port.raw_port()) == 0;
}

QVariant decodeAtom(ei_x_buff *buff, bool *ok)
{
    int type;
    int size;

    ei_get_type(buff->buff, &(buff->index), &type, &size);

    char *namedata = new char[size + 100];
    QVariant val;

    if (ei_decode_atom(buff->buff, &(buff->index), namedata) == 0) {
        QByteArray name(namedata, size);
        val = QVariant::fromValue(ErlAtom(name));
    } else {
        val = QVariant();
        *ok = false;
    }

    delete namedata;

    return val;
}

bool encodeAtom(QVariant var, ei_x_buff *buff)
{
    ErlAtom atom = qvariant_cast<ErlAtom>(var);
    QByteArray name = atom.name();

    return ei_x_encode_atom(buff, name.data()) == 0;
}

}
