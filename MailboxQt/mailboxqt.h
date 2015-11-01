#ifndef MAILBOXQT_H
#define MAILBOXQT_H

#include "mailboxqt_global.h"

#include <QObject>

namespace Mailbox {

void init();

extern unsigned int pidMetaType;
extern unsigned int refMetaType;
extern unsigned int portMetaType;
extern unsigned int atomMetaType;

} // namespace Mailbox

#endif // MAILBOXQT_H
