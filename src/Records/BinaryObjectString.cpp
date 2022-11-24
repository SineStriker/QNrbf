#include "BinaryObjectString.h"

QNRBF_BEGIN_NAMESPACE

BinaryObjectString::BinaryObjectString() {
    objectId = 0;
}

bool BinaryObjectString::read(QDataStream &in) {
    return false;
}


QNRBF_END_NAMESPACE