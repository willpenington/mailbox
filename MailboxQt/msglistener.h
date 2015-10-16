#ifndef MSGLISTENER_H
#define MSGLISTENER_H

#include <QObject>
#include <QThread>

namespace Mailbox {

class MsgListener : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;

public:
    explicit MsgListener(int fd, QObject *parent = 0);

signals:
    void messageRecieved();

public slots:

private:
    int m_fd;
};

} // namespace Mailbox

#endif // MSGLISTENER_H