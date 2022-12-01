#ifndef QNRBFFORMAT_ARRAYOBJECT_H
#define QNRBFFORMAT_ARRAYOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL ArrayObject : public AbstractObject {
public:
    ArrayObject();
    explicit ArrayObject(const QList<ObjectRef> values);
    explicit ArrayObject(QList<ObjectRef> &&values) noexcept;

    QList<ObjectRef> values;
    QList<int> lengths;
    QList<int> lowerBounds;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_ARRAYOBJECT_H
