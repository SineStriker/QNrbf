#ifndef QNRBFFORMAT_DATALISTOBJECT_H
#define QNRBFFORMAT_DATALISTOBJECT_H

#include "AbstractObject.h"
#include "Common/PrimitiveValueArray.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL DataListObject : public AbstractObject {
public:
    DataListObject();

    DataListObject(const PrimitiveValueArray &arr) : DataListObject() {
        values = arr;
    }

    template <class T>
    explicit DataListObject(const QList<T> &list) : DataListObject() {
        values = PrimitiveValueArray(list);
    }

    PrimitiveValueArray values;
    QList<int> lengths;
    QList<int> lowerBounds;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_DATALISTOBJECT_H
