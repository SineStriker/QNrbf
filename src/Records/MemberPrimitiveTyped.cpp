#include "MemberPrimitiveTyped.h"

QNRBF_BEGIN_NAMESPACE

MemberPrimitiveTyped::MemberPrimitiveTyped() {
    primitiveTypeEnum = PrimitiveTypeEnumeration::Null;
}

bool MemberPrimitiveTyped::read(QDataStream &in) {
    if (!Parser::readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
        return false;
    }
    if (!value.read(in, primitiveTypeEnum)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE