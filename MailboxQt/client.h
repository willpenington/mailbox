#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

struct ei_cnode_s;
typedef struct ei_cnode_s ei_cnode;

namespace Mailbox {

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();

    bool connect(QByteArray otherNode, QByteArray name, QByteArray cookie);

    void sendAtom(QByteArray procName, QByteArray atom);

signals:

    void messageRecieved();

public slots:

private:
    int m_fd;
    ei_cnode *m_ec;
};

}

#endif // CLIENT_H
