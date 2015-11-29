#include "mailslotqt.h"

#include "erl_interface.h"

#include "erlpid.h"
#include "erlref.h"
#include "erlport.h"
#include "erlatom.h"

namespace MailSlot {

unsigned int pidMetaType = 0;
unsigned int refMetaType = 0;
unsigned int portMetaType = 0;
unsigned int atomMetaType = 0;

void init() {
  erl_init(NULL, 0);

//  registerPidType();

  pidMetaType = qRegisterMetaType<ErlPid>();
  QMetaType::registerEqualsComparator<ErlPid>();

  refMetaType = qRegisterMetaType<ErlRef>();
  QMetaType::registerEqualsComparator<ErlRef>();

  portMetaType = qRegisterMetaType<ErlPort>();
  QMetaType::registerEqualsComparator<ErlPort>();

  atomMetaType = qRegisterMetaType<ErlAtom>();
  QMetaType::registerEqualsComparator<ErlAtom>();
}

}
