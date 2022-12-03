#ifndef QNRBFFORMAT_PRIMITIVELISTOBJECT_H
#define QNRBFFORMAT_PRIMITIVELISTOBJECT_H

#include "AbstractObject.h"
#include "Common/PrimitiveValueArray.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL PrimitiveListObject : public AbstractObject {
public:
    PrimitiveListObject();

    PrimitiveListObject(const PrimitiveValueArray &arr) : PrimitiveListObject() {
        values = arr;
    }

    template <class T>
    explicit PrimitiveListObject(const QList<T> &list) : PrimitiveListObject() {
        values = PrimitiveValueArray(list);
    }

    PrimitiveValueArray values;
    QList<int> lengths;
    QList<int> lowerBounds;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_PRIMITIVELISTOBJECT_H
