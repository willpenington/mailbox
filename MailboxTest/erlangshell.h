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

#ifndef ERLANGSHELL_H
#define ERLANGSHELL_H

#include <QByteArray>
#include <QProcess>

#define ERL_COMMAND "/usr/bin/erl -sname "

class ErlangShell
{
public:
    ErlangShell(QByteArray sname);
    ~ErlangShell();

    QByteArray execStatement(QByteArray statement);

private:
    QProcess proc;

};

#endif // ERLANGSHELL_H
