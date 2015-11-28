#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QVariant>

#include "msglistener.h"

struct ei_cnode_s;
typedef struct ei_cnode_s ei_cnode;

namespace MailSlot {

class Process;

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

    QVariant self();

    Process *spawn();

signals:

    void messageRecieved(QVariant to, QVariant value);

public slots:
    void listenerMessage(QVariant to, QVariant value);

private:

    int m_fd;
    ei_cnode *m_ec;

    MsgListener *m_listener = nullptr;
};

}

#endif // CLIENT_H
