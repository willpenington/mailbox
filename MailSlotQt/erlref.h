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

#ifndef ERLREF_H
#define ERLREF_H

#include <QObject>
#include <QSharedDataPointer>

namespace MailSlot {

class RefData;

class ErlRef
{
public:
    ErlRef();
    ErlRef(void *raw_ref);
    ErlRef(const ErlRef &other);
    ErlRef(int len, unsigned int a, unsigned int b, unsigned int c,
           unsigned int creation, QString node);

    ~ErlRef();

    ErlRef &operator=(const ErlRef &rhs);
    bool operator==(const ErlRef &other) const;
    inline bool operator !=(const ErlRef &other) const { return !operator==(other); }

    void *raw_ref();

private:
    QSharedDataPointer<RefData> d;
};

}

Q_DECLARE_METATYPE(MailSlot::ErlRef)

#endif // ERLREF_H
