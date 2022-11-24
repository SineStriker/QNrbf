#include "BinaryLibrary.h"

QNRBF_BEGIN_NAMESPACE

BinaryLibrary::BinaryLibrary() {
    libraryId = 0;
}

bool BinaryLibrary::read(QDataStream &in) {
    return false;
}

QNRBF_END_NAMESPACE