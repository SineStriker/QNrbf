#ifndef QNRBFFORMAT_LISTOBJECT_H
#define QNRBFFORMAT_LISTOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API ListObject : public AbstractObject {
public:
    explicit ListObject(Type type);

    QList<int> lengths;
    QList<int> lowerBounds;

    QString arrayType;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_LISTOBJECT_H
