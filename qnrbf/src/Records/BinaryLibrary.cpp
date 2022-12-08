#include "BinaryLibrary.h"

#include "Utils/NrbfParser.h"

QNRBF_BEGIN_NAMESPACE

BinaryLibrary::BinaryLibrary() {
    libraryId = 0;
}

bool BinaryLibrary::read(QDataStream &in) {
    in >> libraryId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (!Parser::readString(libraryName, in)) {
        return false;
    }
    return true;
}

bool BinaryLibrary::write(QDataStream &out) {
    out << libraryId;
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    if (!Parser::writeString(libraryName, out)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE