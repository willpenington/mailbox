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

#ifndef ERLPORT_H
#define ERLPORT_H

#include <QObject>
#include "ei.h"

namespace Mailbox {

class ErlPort
{
public:
    ErlPort();
    ErlPort(erlang_port port);

    erlang_port *port();

    friend bool operator ==(const ErlPort &p1, const ErlPort &p2);

private:
    erlang_port m_port;

};

}

Q_DECLARE_METATYPE(Mailbox::ErlPort)

#endif // ERLPORT_H