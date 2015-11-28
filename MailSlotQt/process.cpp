#include "process.h"

#include "client.h"

namespace MailSlot {

Process::Process(Client *client) :
    m_client(client),
    QObject(client)
{
    QObject::connect(m_client, &Client::messageRecieved,
                     [=](QVariant to, QVariant value) { emit messageRecieved(to, value); });
}

QVariant Process::pid() {
    m_client->self();
}

}
