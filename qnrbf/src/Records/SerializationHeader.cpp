#include "SerializationHeader.h"

QNRBF_BEGIN_NAMESPACE

SerializationHeader::SerializationHeader() {
    rootId = 0;
    headerId = 0;
    majorVersion = 0;
    minorVersion = 0;
}

bool SerializationHeader::read(QDataStream &in) {
    in >> rootId;
    in >> headerId;
    in >> majorVersion;
    in >> minorVersion;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

bool SerializationHeader::write(QDataStream &out) {
    out << rootId;
    out << headerId;
    out << majorVersion;
    out << minorVersion;
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    return false;
}

QNRBF_END_NAMESPACE
