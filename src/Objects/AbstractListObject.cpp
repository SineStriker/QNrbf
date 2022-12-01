#include "AbstractListObject.h"

QNRBF_BEGIN_NAMESPACE

AbstractListObject::AbstractListObject() : AbstractObject(ObjectList) {
}

AbstractListObject::AbstractListObject(const QList<ObjectRef> values)
    : AbstractObject(ObjectList), values(values) {
}

AbstractListObject::AbstractListObject(QList<ObjectRef> &&values) noexcept
    : AbstractObject(ObjectList), values(values) {
}

QNRBF_END_NAMESPACE
