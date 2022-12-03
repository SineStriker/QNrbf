#include "ClassWithId.h"

QNRBF_BEGIN_NAMESPACE

ClassWithId::ClassWithId() {
    objectId = 0;
    metadataId = 0;
}

bool ClassWithId::read(QDataStream &in) {
    in >> objectId;
    in >> metadataId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE