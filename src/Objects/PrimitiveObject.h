#ifndef QNRBFFORMAT_PRIMITIVEOBJECT_H
#define QNRBFFORMAT_PRIMITIVEOBJECT_H

#include "AbstractObject.h"
#include "Common/PrimitiveValue.h"
#include "Enums/PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL PrimitiveObject : public AbstractObject {
public:
    explicit PrimitiveObject(const PrimitiveValue &val);

    PrimitiveValue value;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_PRIMITIVEOBJECT_H
