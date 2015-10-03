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

#include "erlangshell.h"

ErlangShell::ErlangShell()
{
    proc.start(ERL_COMMAND);

    // Ignore the REPL Header Infomation
    proc.waitForReadyRead();
}

ErlangShell::~ErlangShell() {
    proc.kill();
    proc.waitForFinished();
}

QByteArray ErlangShell::execStatement(QByteArray statement) {

    // Ignore the REPL command prompt (>) and any older data in the stream
    proc.waitForReadyRead();
    proc.readAll();

    // Write the statement to standard in
    proc.write(statement);

    // Send Newline to force evaluation
    proc.write("\n");

    QByteArray response;

    /* Read from stdout till a string ending in a newline is found. The Erlang
     * VM writes what is has when it has it and sometimes it takes a few
     * sections to get the full response. */
    do {
        proc.waitForReadyRead();
        response.append(proc.readLine());
    } while (!(response.endsWith('\n')));

    return response;
}
