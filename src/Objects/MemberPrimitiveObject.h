#ifndef QNRBFFORMAT_MEMBERPRIMITIVEOBJECT_H
#define QNRBFFORMAT_MEMBERPRIMITIVEOBJECT_H

#include "AbstractObject.h"
#include "Enums/PrimitiveTypeEnumeration.h"
#include "Records/MemberPrimitiveTyped.h"

QNRBF_BEGIN_NAMESPACE

class MemberPrimitiveObject : public AbstractObject {
public:
    MemberPrimitiveObject();
    MemberPrimitiveObject(const MemberPrimitiveTyped &record);

    PrimitiveTypeEnumeration primitiveTypeEnum;
    QSharedPointer<AbstractObject> value;

    void fromRecord(const MemberPrimitiveTyped &record);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_MEMBERPRIMITIVEOBJECT_H
