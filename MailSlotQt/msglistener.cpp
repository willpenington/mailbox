#include "msglistener.h"

#include <ei_connect.h>

#include "erlconversion.h"

namespace MailSlot {

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

    do {
        ei_x_new(&buff);
        length = ei_xreceive_msg_tmo(m_fd, &msg, &buff, 1000);

        if (length > 0 && (msg.msgtype == ERL_SEND || msg.msgtype == ERL_REG_SEND)) {
            // Reset the buffer to read from the start
            buff.index = 0;

            // Read off the version magic number to shift the index
            int version;
            ei_decode_version(buff.buff, &buff.index, &version);

            bool ok = true;
            QVariant var = decode(&buff, &ok);

            if (ok)
                emit messageRecieved(var);
        }

        ei_x_free(&buff);

    } while (length > 0); // TODO: Replace with proper notification
}


} // namespace MailSlot
