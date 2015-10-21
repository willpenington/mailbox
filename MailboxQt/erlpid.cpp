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

#include "erlpid.h"

namespace Mailbox {

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
    return (p1.m_pid.creation == p2.m_pid.creation)
        && (p1.m_pid.num == p2.m_pid.num)
        && (p1.m_pid.serial == p2.m_pid.serial)
        && (strcmp(p1.m_pid.node, p2.m_pid.node) == 0);

}

}
