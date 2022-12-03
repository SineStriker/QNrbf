#include "OneOrMoreNullObject.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::OneOrMoreNullObject::OneOrMoreNullObject() : OneOrMoreNullObject(1) {
}

OneOrMoreNullObject::OneOrMoreNullObject(qint32 nullCount)
    : AbstractObject(Null), nullCount(nullCount) {
}

QNRBF_END_NAMESPACE