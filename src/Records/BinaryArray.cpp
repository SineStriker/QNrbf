#include "BinaryArray.h"

QNRBF_BEGIN_NAMESPACE

BinaryArray::BinaryArray() {
    objectId = 0;
    binaryArrayTypeEnum = BinaryArrayTypeEnumeration::Single;
    rank = 0;
}

bool BinaryArray::read(QDataStream &in) {
    return false;
}

QNRBF_END_NAMESPACE