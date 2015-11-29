#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QVariant>

namespace MailSlot {

class Client;

class Process : public QObject
{
    Q_OBJECT
public:
    explicit Process(Client *client, QVariant pid);

    QVariant pid();

//    void processMessage(QVariant procName, QVariant value);

signals:
    void messageRecieved(QVariant procName, QVariant value);

public slots:

private:
    Client *m_client;
    QVariant m_pid;
};

}

#endif // PROCESS_H
