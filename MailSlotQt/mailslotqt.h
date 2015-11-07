#ifndef MAILSLOTQT_H
#define MAILSLOTQT_H

#include "mailslotqt_global.h"

#include <QObject>

namespace MailSlot {

void init();

extern unsigned int pidMetaType;
extern unsigned int refMetaType;
extern unsigned int portMetaType;
extern unsigned int atomMetaType;

QString formatErlangTerm(QVariant var);

} // namespace MailSlot

#endif // MAILSLOTQT_H
