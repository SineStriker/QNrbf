#include "StringValueWithCode.h"

#include "Utils/NrbfParser.h"

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

bool StringValueWithCode::write(QDataStream &out) const {
    if (!Parser::writePrimitiveTypeEnum(primitiveTypeEnum, out)) {
        return false;
    }
    if (!Parser::writeString(stringValue, out)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
