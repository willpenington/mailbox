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

#include "erlport.h"

#include <ei.h>

#include <QSharedData>

namespace MailSlot {

class PortData : public QSharedData {
public:
    PortData() { }
    PortData(const PortData &other)
        : QSharedData(other), port(other.port) { }
    ~PortData() { }

    erlang_port port;
};

ErlPort::ErlPort()
{
    d = new PortData;
}

ErlPort::ErlPort(const ErlPort &other) :
    d(other.d)
{

}

ErlPort::ErlPort(void *raw_port)
{
    d = new PortData;
    d->port = *((erlang_port *) raw_port);
}

ErlPort::ErlPort(unsigned int id, unsigned int creation, QString node)
{
    d = new PortData;
    d->port.id = id;
    d->port.creation = creation;
    strncpy(d->port.node, node.toUtf8().data(), MAXATOMLEN_UTF8);
}

ErlPort &ErlPort::operator=(const ErlPort &rhs) {
    if (this == &rhs) return *this;
    d = rhs.d;
    return *this;
}

ErlPort::~ErlPort() {

}

void *ErlPort::raw_port()
{
    return &(d->port);
}

bool ErlPort::operator==(const ErlPort &other) const
{
    return ((((d->port.creation) & 0x3) == (other.d->port.creation) & 0x3))
        && (((d->port.id & 0x3FFFF) == (other.d->port.id) & 0x3FFFF))
        && (strcmp(d->port.node, other.d->port.node) == 0);
}

}

