#ifndef QNRBF_MEMBERPRIMITIVETYPED_H
#define QNRBF_MEMBERPRIMITIVETYPED_H

#include "Common/PrimitiveValue.h"
#include "Enums/PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

// 2.5.1 MemberPrimitiveTyped

class QNRBF_INTERNAL MemberPrimitiveTyped {
public:
    MemberPrimitiveTyped();

    PrimitiveValue value;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_MEMBERPRIMITIVETYPED_H
