#ifndef QNRBFFORMAT_PRIMITIVELISTOBJECT_H
#define QNRBFFORMAT_PRIMITIVELISTOBJECT_H

#include "ListObject.h"
#include "Common/PrimitiveValueArray.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL PrimitiveListObject : public ListObject {
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
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_PRIMITIVELISTOBJECT_H
