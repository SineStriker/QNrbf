#ifndef QNRBF_ARRAYSINGLEPRIMITIVE_H
#define QNRBF_ARRAYSINGLEPRIMITIVE_H

#include "Structures/ArrayInfo.h"
#include "Enums/PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

// 2.4.3.3 ArraySinglePrimitive
class QNRBF_INTERNAL ArraySinglePrimitive {
public:
    ArraySinglePrimitive();

    ArrayInfo arrayInfo;
    PrimitiveTypeEnumeration primitiveTypeEnum;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_ARRAYSINGLEPRIMITIVE_H
