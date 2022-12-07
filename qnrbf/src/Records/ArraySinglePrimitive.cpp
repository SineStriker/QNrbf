#include "ArraySinglePrimitive.h"

QNRBF_BEGIN_NAMESPACE

ArraySinglePrimitive::ArraySinglePrimitive() {
    primitiveTypeEnum = PrimitiveTypeEnumeration::None;
}

bool ArraySinglePrimitive::read(QDataStream &in) {
    if (!arrayInfo.read(in)) {
        return false;
    }
    if (!Parser::readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
        return false;
    }
    return true;
}

bool ArraySinglePrimitive::write(QDataStream &out) const {
    if (!arrayInfo.write(out)) {
        return false;
    }
    if (!Parser::writePrimitiveTypeEnum(primitiveTypeEnum, out)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE