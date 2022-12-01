#include "StringListObject.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::StringListObject::StringListObject(const QStringList &arr)
    : AbstractObject(StringList), values(arr) {
}

QNRBF_END_NAMESPACE