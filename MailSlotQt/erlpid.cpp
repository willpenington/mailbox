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

#define LOWER(value, usedsize) ((value << (sizeof(value) - usedsize )) >> (sizeof(value) - usedsize))

namespace MailSlot {

ErlPid::ErlPid()
{

}

ErlPid::ErlPid(erlang_pid pid) :
    m_pid(pid)
{

}

erlang_pid *ErlPid::pid()
{
    return &m_pid;
}

bool operator ==(const ErlPid &p1, const ErlPid &p2)
{
    int creation1 = p1.m_pid.creation & 0x03;
    int creation2 = p2.m_pid.creation & 0x03;
    int serial1 = p1.m_pid.serial & 0x07;
    int serial2 = p2.m_pid.serial & 0x07;
    int number1 = p1.m_pid.num & 0x7FFF;
    int number2 = p2.m_pid.num & 0x7FFF;

    return (creation1 == creation2)
        && (serial1 == serial2)
        && (number1 == number2)
        && (strcmp(p1.m_pid.node, p2.m_pid.node) == 0);

    return (LOWER(p1.m_pid.creation,2) == LOWER(p2.m_pid.creation, 2))
        && (p1.m_pid.num == p2.m_pid.num)
        && (p1.m_pid.serial == p2.m_pid.serial)
        && (strcmp(p1.m_pid.node, p2.m_pid.node) == 0);

}

}
