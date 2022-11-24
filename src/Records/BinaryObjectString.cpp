#include "BinaryObjectString.h"

#include "Primitive/LengthPrefixedString.h"

QNRBF_BEGIN_NAMESPACE

BinaryObjectString::BinaryObjectString() {
    objectId = 0;
}

bool BinaryObjectString::read(QDataStream &in) {
    in >> objectId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (!Parser::readString(value, in)) {
        return false;
    }
    return true;
}


QNRBF_END_NAMESPACE