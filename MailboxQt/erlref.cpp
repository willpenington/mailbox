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

#include "erlref.h"

namespace Mailbox {

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

bool operator ==(const ErlRef &r1, const ErlRef &r2)
{
    return (r1.m_ref.creation == r2.m_ref.creation)
        && (r1.m_ref.len == r2.m_ref.len)
        && (memcmp(r1.m_ref.n, r2.m_ref.n, r1.m_ref.len) == 0)
        && (strcmp(r1.m_ref.node, r2.m_ref.node) == 0);

}


}

