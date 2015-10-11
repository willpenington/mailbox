#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

namespace Mailbox {

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    bool connect(QByteArray otherNode, QByteArray name);

    void sendAtom(QByteArray procName, QByteArray atom);

signals:

    void messageRecieved();

public slots:
};

}

#endif // CLIENT_H
