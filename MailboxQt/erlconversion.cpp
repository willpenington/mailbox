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

#include "erlconversion.h"

namespace Mailbox {
bool encodeDouble(QVariant var, ei_x_buff *buff);
QVariant decodeDouble(ei_x_buff *buff, bool *ok);

bool encodeInt(QVariant var, ei_x_buff *buff);
QVariant decodeInt(ei_x_buff *buff, bool *ok);

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
        return decodeInt(buff, ok);
    default:
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
    case QMetaType::Int:
    case QMetaType::Long:
    case QMetaType::Short:
        return encodeInt(var, buff);
    default:
        break;
    }

    return false;
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
    long val;

    if (ei_decode_long(buff->buff, &(buff->index), &val) != 0) {
        *ok = false;
        return QVariant();
    }

    return QVariant::fromValue(val);
}

bool encodeInt(QVariant var, ei_x_buff *buff)
{
    bool convert_ok = true;
    long val = qvariant_cast<long>(var);

    if (!convert_ok)
        return false;

    return (ei_x_encode_double(buff, val) == 0);
}

}
