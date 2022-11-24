#include "ClassWithId.h"

QNRBF_BEGIN_NAMESPACE

ClassWithId::ClassWithId() {
    objectId = 0;
    metadataId = 0;
}

bool ClassWithId::read(QDataStream &in) {
    return false;
}

QNRBF_END_NAMESPACE