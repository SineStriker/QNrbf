#include "StringObject.h"

QNRBF_BEGIN_NAMESPACE

StringObject::StringObject(const QString &val)
    : AbstractObject(String), value(val) {
}

QNRBF_END_NAMESPACE