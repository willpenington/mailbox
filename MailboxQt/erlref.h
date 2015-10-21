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

#ifndef ERLREF_H
#define ERLREF_H

#include <QObject>
#include "ei.h"

namespace Mailbox {

class ErlRef
{
public:
    ErlRef();
    ErlRef(erlang_ref ref);

    erlang_ref *ref();

    friend bool operator ==(const ErlRef &r1, const ErlRef &r2);

private:
    erlang_ref m_ref;
};

}

Q_DECLARE_METATYPE(Mailbox::ErlRef)

#endif // ERLREF_H
