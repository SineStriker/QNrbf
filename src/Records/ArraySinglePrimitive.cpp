#include "ArraySinglePrimitive.h"

QNRBF_BEGIN_NAMESPACE

ArraySinglePrimitive::ArraySinglePrimitive() {
    primitiveTypeEnum = PrimitiveTypeEnumeration::None;
}

bool ArraySinglePrimitive::read(QDataStream &in) {
    return false;
}

QNRBF_END_NAMESPACE