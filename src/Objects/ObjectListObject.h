#ifndef QNRBFFORMAT_OBJECTLISTOBJECT_H
#define QNRBFFORMAT_OBJECTLISTOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL ObjectListObject : public AbstractObject {
public:
    ObjectListObject();
    explicit ObjectListObject(const QList<ObjectRef> values);
    explicit ObjectListObject(QList<ObjectRef> &&values) noexcept;

    QList<ObjectRef> values;
    QList<int> lengths;
    QList<int> lowerBounds;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_OBJECTLISTOBJECT_H
