#include "BinaryObjectString.h"

#include "Primitive/Parser.h"

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

bool BinaryObjectString::write(QDataStream &out) const {
    out << objectId;
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    if (!Parser::writeString(value, out)) {
        return false;
    }
    return true;
}


QNRBF_END_NAMESPACE