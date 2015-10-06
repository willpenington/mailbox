#include "mailboxqt.h"

#include "erl_interface.h"

namespace Mailbox {

void init() {
  erl_init(NULL, 0);
}

}
