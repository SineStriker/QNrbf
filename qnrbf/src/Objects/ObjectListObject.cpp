#include "ObjectListObject.h"

QNRBF_BEGIN_NAMESPACE

ObjectListObject::ObjectListObject() : ListObject(ObjectList) {
}

ObjectListObject::ObjectListObject(const QList<ObjectRef> &values)
    : ListObject(ObjectList), values(values) {
}

ObjectListObject::ObjectListObject(QList<ObjectRef> &&values) noexcept
    : ListObject(ObjectList), values(values) {
}

QNRBF_END_NAMESPACE
