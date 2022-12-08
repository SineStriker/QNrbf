#include "ClassWithId.h"

#include <QDebug>

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

bool ClassWithId::write(QDataStream &out) const {
    out << objectId;
    out << metadataId;
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE