#include "ValueWithCode.h"

QNRBF_BEGIN_NAMESPACE

ValueWithCode::ValueWithCode() {
    primitiveTypeEnum = PrimitiveTypeEnumeration::Null;
}

bool ValueWithCode::read(QDataStream &in) {
    if (!Parser::readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
        return false;
    }
    if (!value.read(in, primitiveTypeEnum)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE