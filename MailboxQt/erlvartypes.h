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

#ifndef ERLVARTYPES_H
#define ERLVARTYPES_H

#include <QVariant>

namespace Mailbox {
// A set of functions for generating Erlang types without importing ei

QVariant build_erl_pid(unsigned int creation, unsigned int num, unsigned int serial, QString node);
QVariant build_erl_port(unsigned int id, unsigned int creation, QString node);
QVariant build_erl_ref(int len, unsigned int a, unsigned int b, unsigned int c, unsigned int creation, QString node);


}


#endif // ERLVARTYPES_H
