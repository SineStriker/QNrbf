#ifndef QNRBFFORMAT_DATALISTOBJECT_H
#define QNRBFFORMAT_DATALISTOBJECT_H

#include "AbstractObject.h"
#include "Common/PrimitiveValue.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL DataListObject : public AbstractObject {
public:
    DataListObject();

    explicit DataListObject(QList<PrimitiveValue> &&list) : DataListObject() {
        values = list;
    }

    template <class T>
    explicit DataListObject(const QList<T> &list) : DataListObject() {
        add(list);
    }

    QList<PrimitiveValue> values;
    QList<int> lengths;
    QList<int> lowerBounds;

    template <class T>
    void add(const QList<T> &list) {
        for (const auto &val : list) {
            values.append(val);
        }
    }
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_DATALISTOBJECT_H
