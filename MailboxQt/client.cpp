#include "client.h"

#include "ei_connect.h"

namespace Mailbox {

Client::Client(QObject *parent) : QObject(parent)
{

}

void Client::sendAtom(QByteArray procName, QByteArray atom)
{

}

bool Client::connect(QByteArray otherNode, QByteArray name)
{
    return true;
}

}
