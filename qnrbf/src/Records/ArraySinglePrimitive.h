#ifndef QNRBF_ARRAYSINGLEPRIMITIVE_H
#define QNRBF_ARRAYSINGLEPRIMITIVE_H

#include "Enums/PrimitiveTypeEnumeration.h"
#include "Structures/ArrayInfo.h"

QNRBF_BEGIN_NAMESPACE

// 2.4.3.3 ArraySinglePrimitive
class QNRBF_INTERNAL ArraySinglePrimitive {
public:
    ArraySinglePrimitive();

    ArrayInfo arrayInfo;
    PrimitiveTypeEnumeration primitiveTypeEnum;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_ARRAYSINGLEPRIMITIVE_H
