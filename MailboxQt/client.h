#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include "msglistener.h"

struct ei_cnode_s;
typedef struct ei_cnode_s ei_cnode;

namespace Mailbox {

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();

    bool connect(QByteArray name, QByteArray nodeName, QByteArray cookie);

    void sendAtom(QByteArray procName, QByteArray atom);
    void sendPid(QByteArray procName);

    void sendMessage(QByteArray procName, QVariant value);

signals:

    void messageRecieved();

public slots:
    void listenerMessage();

private:

    int m_fd;
    ei_cnode *m_ec;

    MsgListener *m_listener = nullptr;
};

}

#endif // CLIENT_H
