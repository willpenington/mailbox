#include "msglistener.h"

#include <ei_connect.h>

namespace Mailbox {

MsgListener::MsgListener(int fd, QObject *parent) :
    QThread(parent),
    m_fd(fd)
{

}

void MsgListener::run()
{

}

} // namespace Mailbox
