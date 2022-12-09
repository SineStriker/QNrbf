#ifndef QNRBFFORMAT_LISTOBJECT_H
#define QNRBFFORMAT_LISTOBJECT_H

#include "AbstractObject.h"
#include "Records/BinaryArray.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL ListObject : public AbstractObject {
public:
    explicit ListObject(Type type);

    QList<qint32> lengths;
    QList<qint32> lowerBounds;

    bool hasShapeInfo;
    BinaryArray shapeInfo;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_LISTOBJECT_H
