#include "StringValueWithCode.h"

#include "Primitive/Parser.h"

QNRBF_BEGIN_NAMESPACE

StringValueWithCode::StringValueWithCode() {
    primitiveTypeEnum = PrimitiveTypeEnumeration::Null;
}

bool StringValueWithCode::read(QDataStream &in) {
    if (!Parser::readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
        return false;
    }
    if (!Parser::readString(stringValue, in)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
