#ifndef QNRBF_VALUEWITHCODE_H
#define QNRBF_VALUEWITHCODE_H

#include "Common/PrimitiveValue.h"
#include "Enums/PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

// 2.2.2.1 ValueWithCode

class QNRBF_API ValueWithCode {
public:
    ValueWithCode();

    PrimitiveTypeEnumeration primitiveTypeEnum;
    PrimitiveValue value;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_VALUEWITHCODE_H
