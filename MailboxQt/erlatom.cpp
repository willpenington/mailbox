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

#include "erlatom.h"

namespace Mailbox {

ErlAtom::ErlAtom() :
    m_name("")
{

}

ErlAtom::ErlAtom(QByteArray name) :
    m_name(name)
{

}

QByteArray ErlAtom::name()
{
    return m_name;
}

bool operator ==(const ErlAtom &a1, const ErlAtom &a2)
{
    return a1.m_name == a2.m_name;
}

}
