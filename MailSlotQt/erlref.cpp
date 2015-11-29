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

#include <ei.h>

#include <QSharedData>

namespace MailSlot {

class RefData : public QSharedData {
public:
    RefData() { }
    RefData(const RefData &other)
        : QSharedData(other), erl_ref(other.erl_ref) { }
    ~RefData() { }

    erlang_ref erl_ref;
};

ErlRef::ErlRef()
{
    d = new RefData;
}

ErlRef::ErlRef(const ErlRef &other) :
    d(other.d)
{

}

ErlRef::ErlRef(void *raw_ref)
{
    d = new RefData;
    d->erl_ref = *((erlang_ref *) raw_ref);
}

ErlRef &ErlRef::operator=(const ErlRef &rhs) {
    if (this == &rhs) return *this;
    d = rhs.d;
    return *this;
}

ErlRef::~ErlRef() {

}

void *ErlRef::raw_ref()
{
    return &(d->erl_ref);
}

bool ErlRef::operator ==(const ErlRef &other) const
{
    return ((d->erl_ref.creation & 0x3) == (other.d->erl_ref.creation) & 0x3)
        && (d->erl_ref.len == other.d->erl_ref.len)
        && (d->erl_ref.len < 1 || (d->erl_ref.n[0] == other.d->erl_ref.n[0]))
        && (d->erl_ref.len < 2 || (d->erl_ref.n[1] == other.d->erl_ref.n[1]))
        && (d->erl_ref.len < 3 || ((d->erl_ref.n[1] & 0x3FFFF) == (other.d->erl_ref.n[1] & 0x3FFFF)))
        && (strcmp(d->erl_ref.node, other.d->erl_ref.node) == 0);

}


}

