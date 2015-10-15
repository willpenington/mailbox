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
    int length;
    erlang_msg msg;
    ei_x_buff buff;

    ei_x_new(&buff);

    length = ei_xreceive_msg_tmo(m_fd, &msg, &buff, 1000);

    if (length > 0 && (msg.msgtype == ERL_SEND || msg.msgtype == ERL_REG_SEND))
        emit messageRecieved();

    ei_x_free(&buff);
}

} // namespace Mailbox
