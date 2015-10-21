#include "mailboxqt.h"

#include "erl_interface.h"

#include "erlpid.h"
#include "erlref.h"

namespace Mailbox {

unsigned int pidMetaType = 0;
unsigned int refMetaType = 0;

void init() {
  erl_init(NULL, 0);

  pidMetaType = qRegisterMetaType<ErlPid>();
  QMetaType::registerEqualsComparator<ErlPid>();

  refMetaType = qRegisterMetaType<ErlRef>();
  QMetaType::registerEqualsComparator<ErlRef>();
}

}
