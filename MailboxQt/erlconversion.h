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

#ifndef ERLCONVERSION_H
#define ERLCONVERSION_H

#include "ei.h"

#include <QVariant>

namespace Mailbox {

QVariant decode(ei_x_buff *buff, bool *ok);
bool encode(QVariant var, ei_x_buff *buff);


bool isErlangTerm(QVariant var);

}

#endif // ERLCONVERSION_H
