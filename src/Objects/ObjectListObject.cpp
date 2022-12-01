#include "ArrayObject.h"

QNRBF_BEGIN_NAMESPACE

ArrayObject::ArrayObject() : AbstractObject(ObjectList) {
}

ArrayObject::ArrayObject(const QList<ObjectRef> values)
    : AbstractObject(ObjectList), values(values) {
}

ArrayObject::ArrayObject(QList<ObjectRef> &&values) noexcept
    : AbstractObject(ObjectList), values(values) {
}

QNRBF_END_NAMESPACE
