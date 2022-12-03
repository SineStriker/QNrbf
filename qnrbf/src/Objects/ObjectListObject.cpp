#include "ObjectListObject.h"

QNRBF_BEGIN_NAMESPACE

ObjectListObject::ObjectListObject() : AbstractObject(ObjectList) {
}

ObjectListObject::ObjectListObject(const QList<ObjectRef> values)
    : AbstractObject(ObjectList), values(values) {
}

ObjectListObject::ObjectListObject(QList<ObjectRef> &&values) noexcept
    : AbstractObject(ObjectList), values(values) {
}

QNRBF_END_NAMESPACE
