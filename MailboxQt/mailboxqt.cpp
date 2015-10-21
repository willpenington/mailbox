#include "mailboxqt.h"

#include "erl_interface.h"

#include "erlpid.h"

namespace Mailbox {

unsigned int pidMetaType = 0;

void init() {
  erl_init(NULL, 0);

  pidMetaType = qRegisterMetaType<ErlPid>();
  QMetaType::registerEqualsComparator<ErlPid>();
}

}
