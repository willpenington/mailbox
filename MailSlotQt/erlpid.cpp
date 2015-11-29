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

#include "erlpid.h"

#include <ei.h>

#include <QSharedData>

#define LOWER(value, usedsize) ((value << (sizeof(value) - usedsize )) >> (sizeof(value) - usedsize))

namespace MailSlot {

class PidData : public QSharedData {
public:
    PidData() { }
    PidData(const PidData &other)
        : QSharedData(other), pid(other.pid) { }
    ~PidData() { }

    erlang_pid pid;
};

ErlPid::ErlPid()
{
    d = new PidData;
}

ErlPid::ErlPid(const ErlPid &other) :
    d(other.d)
{

}

ErlPid::ErlPid(void *raw_pid)
{
    d = new PidData;
    d->pid = *((erlang_pid *) raw_pid);
}

ErlPid::ErlPid(unsigned int creation, unsigned int num, unsigned int serial, QString node)
{
    d = new PidData;

    d->pid.creation = creation;
    d->pid.num = num;
    d->pid.serial = serial;

    strncpy(d->pid.node, node.toUtf8().data(), MAXATOMLEN_UTF8);
}

ErlPid &ErlPid::operator =(const ErlPid &rhs) {
    if (this == &rhs) return *this;
    d = rhs.d;
    return *this;
}

ErlPid::~ErlPid() {

}

void *ErlPid::raw_pid()
{
    return  &(d->pid);
}



bool ErlPid::operator ==(const ErlPid &other) const
{

    int creation1 = d->pid.creation & 0x03;
    int creation2 = other.d->pid.creation & 0x03;
    int serial1 = d->pid.serial & 0x07;
    int serial2 = other.d->pid.serial & 0x07;
    int number1 = d->pid.num & 0x7FFF;
    int number2 = other.d->pid.num & 0x7FFF;

    return (creation1 == creation2)
        && (serial1 == serial2)
        && (number1 == number2)
        && (strcmp(d->pid.node, other.d->pid.node) == 0);

}


}


void MailSlot::registerPidType() {
//  pidMetaType = qRegisterMetaType<ErlPid>();
//  QMetaType::registerEqualsComparator<ErlPid>();
}
