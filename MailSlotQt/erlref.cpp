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

#include "erlref.h"

namespace MailSlot {

ErlRef::ErlRef()
{

}

ErlRef::ErlRef(erlang_ref ref) :
    m_ref(ref)
{
}

erlang_ref *ErlRef::ref()
{
    return &m_ref;
}

ErlRef::~ErlRef() {
}

bool operator ==(const ErlRef &r1, const ErlRef &r2)
{
    return ((r1.m_ref.creation & 0x3) == (r2.m_ref.creation) & 0x3)
        && (r1.m_ref.len == r2.m_ref.len)
        && (r1.m_ref.len < 1 || (r1.m_ref.n[0] == r2.m_ref.n[0]))
        && (r1.m_ref.len < 2 || (r1.m_ref.n[1] == r2.m_ref.n[1]))
        && (r1.m_ref.len < 3 || ((r1.m_ref.n[1] & 0x3FFFF) == (r2.m_ref.n[1] & 0x3FFFF)))
        && (strcmp(r1.m_ref.node, r2.m_ref.node) == 0);

}


}

