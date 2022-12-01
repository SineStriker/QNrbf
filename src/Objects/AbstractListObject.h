#ifndef QNRBFFORMAT_ABSTRACTLISTOBJECT_H
#define QNRBFFORMAT_ABSTRACTLISTOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL AbstractListObject : public AbstractObject {
public:
    AbstractListObject();
    explicit AbstractListObject(const QList<ObjectRef> values);
    explicit AbstractListObject(QList<ObjectRef> &&values) noexcept;

    QList<ObjectRef> values;
    QList<int> lengths;
    QList<int> lowerBounds;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_ABSTRACTLISTOBJECT_H
