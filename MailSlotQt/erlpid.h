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

#ifndef ERLPID_H
#define ERLPID_H

#include <QObject>
#include <QSharedDataPointer>

namespace MailSlot {

class PidData;

class ErlPid
{
public:
    ErlPid();
    ErlPid(void *raw_pid);
    ErlPid(const ErlPid &other);
    ~ErlPid();

    ErlPid &operator=(const ErlPid &rhs);
    bool operator==(const ErlPid &other) const;
    inline bool operator !=(const ErlPid &other) const { return !operator==(other); }

    void *raw_pid();

//    friend bool operator ==(const ErlPid &p1, const ErlPid &p2);

private:
    QSharedDataPointer<PidData> d;
//    erlang_pid m_pid;

};

void registerPidType();

}

Q_DECLARE_METATYPE(MailSlot::ErlPid)

#endif // ERLPID_H
