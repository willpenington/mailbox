#include "process.h"

#include "client.h"

namespace MailSlot {

Process::Process(Client *client, QVariant pid) :
    m_client(client),
    m_pid(pid),
    QObject(client)
{
    QObject::connect(m_client, &Client::messageRecieved,
                     [=](QVariant to, QVariant value) {
        if (to == this->m_pid)
            emit messageRecieved(to, value);
    });
}

QVariant Process::pid() {
    return m_pid;
}

//void Process::processMessage(QVariant procName, QVariant value) {
//    if (procName == m_pid)
//        emit messageRecieved(procName, value);
//}

}
