#ifndef QNRBFFORMAT_MEMBERPRIMITIVEOBJECT_H
#define QNRBFFORMAT_MEMBERPRIMITIVEOBJECT_H

#include "AbstractObject.h"
#include "Common/PrimitiveValue.h"
#include "Enums/PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL MemberPrimitiveObject : public AbstractObject {
public:
    explicit MemberPrimitiveObject(const PrimitiveValue &val);

    PrimitiveValue value;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_MEMBERPRIMITIVEOBJECT_H
