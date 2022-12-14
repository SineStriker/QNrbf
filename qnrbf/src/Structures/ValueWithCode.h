#ifndef QNRBF_VALUEWITHCODE_H
#define QNRBF_VALUEWITHCODE_H

#include "Common/PrimitiveValue.h"
#include "Enums/PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

// 2.2.2.1 ValueWithCode

class QNRBF_INTERNAL ValueWithCode {
public:
    ValueWithCode();

    PrimitiveValue value;

    bool read(QDataStream &in);
    bool write(QDataStream &out)const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_VALUEWITHCODE_H
