#include "client.h"

#include <unistd.h>

#include "ei_connect.h"

#include "erlconversion.h"
#include "erlpid.h"
#include "process.h"

#include <QDebug>


namespace MailSlot {

short creation = 0;

Client::Client(QObject *parent) : QObject(parent)
{
    m_ec = new ei_cnode();
    Q_ASSERT(m_ec != nullptr);

}

Client::~Client()
{
    close(m_fd);

    if (m_listener != nullptr)
    {
        m_listener->quit();
        m_listener->wait();
    }

    delete m_ec;
}

void Client::sendAtom(QByteArray procName, QByteArray atom)
{
    ei_x_buff x;
    ei_x_new_with_version(&x);

    ei_x_encode_atom(&x, atom.data());

    ei_reg_send(m_ec, m_fd, procName.data(), x.buff, x.index);

    ei_x_free(&x);
}

void Client::sendPid(QByteArray procName)
{
    ei_x_buff x;
    ei_x_new_with_version(&x);

    ei_x_encode_pid(&x, &(m_ec->self));

    ei_reg_send(m_ec, m_fd, procName.data(), x.buff, x.index);

    ei_x_free(&x);
}

void Client::sendMessage(QByteArray procName, QVariant value)
{
    ei_x_buff x;
    ei_x_new_with_version(&x);

    encode(value, &x);
    ei_reg_send(m_ec, m_fd, procName.data(), x.buff, x.index);

    ei_x_free(&x);
}

QVariant Client::self()
{
    return QVariant::fromValue(ErlPid((void *) &(m_ec->self)));
}

void Client::listenerMessage(QVariant to, QVariant value)
{
    emit messageRecieved(to, value);
}

bool Client::connect(QByteArray name, QByteArray otherNode, QByteArray cookie)
{

    if (name == otherNode)
    {
        qWarning() << "Node and otherNode names are the same";
        return false;
    }

    if(ei_connect_init(m_ec, name.data(), cookie.data(), creation++) < 0)
    {
        qWarning() << "Error initialising Erlang CNode";
        return false;
    }

    QByteArray fullName = otherNode + "@localHost";

    int fd = ei_connect(m_ec, fullName.data());
    if (fd < 0)
    {
        qWarning() << "Error connecting to Erlang Node";
        switch (erl_errno) {
        case EHOSTUNREACH:
            qWarning() << "Host is unreachable";
            break;
        case ENOMEM:
            qWarning() << "Out of Memory";
            break;
        case EIO:
            qWarning() << "IO Error";
            break;
        default:
            qWarning() << "erl_errno code" << fd;
            break;
        }
        return false;
    }

    m_fd = fd;

    m_listener = new MsgListener(fd, this);

    QObject::connect(m_listener, &MsgListener::finished, m_listener, &QObject::deleteLater);

    QObject::connect(m_listener, &MsgListener::messageRecieved,
                     [=](QVariant to, QVariant value) { emit messageRecieved(to, value); });

    m_listener->start();

    return true;
}

Process *Client::spawn() {

    static unsigned int num_counter = m_ec->self.num;
    static unsigned int serial_counter = m_ec->self.serial;

    erlang_pid pid = m_ec->self;

    num_counter++;

    pid.num = num_counter;

    QVariant var = QVariant::fromValue(ErlPid((void *) &pid));

    return new Process(this, var);
}

}
