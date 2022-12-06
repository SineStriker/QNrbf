#ifndef QNRBFFORMAT_OBJECTLISTOBJECT_H
#define QNRBFFORMAT_OBJECTLISTOBJECT_H

#include "ListObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL ObjectListObject : public ListObject {
public:
    ObjectListObject();
    ObjectListObject(const QList<ObjectRef> &values);
    ObjectListObject(QList<ObjectRef> &&values) noexcept;

    QList<ObjectRef> values;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_OBJECTLISTOBJECT_H
