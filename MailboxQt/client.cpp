#include "client.h"

#include "ei_connect.h"

namespace Mailbox {

Client::Client(QObject *parent) : QObject(parent)
{
    m_ec = new ei_cnode();
    Q_ASSERT(m_ec != nullptr);
}

Client::~Client()
{
    delete m_ec;
}

void Client::sendAtom(QByteArray procName, QByteArray atom)
{

}

bool Client::connect(QByteArray otherNode, QByteArray name, QByteArray cookie)
{
    return true;
}

}
