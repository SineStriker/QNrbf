#include "ClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::ClassWithMembersAndTypes::ClassWithMembersAndTypes() {
    libraryId = 0;
}

bool ClassWithMembersAndTypes::read(QDataStream &in) {
    return false;
}

QNRBF_END_NAMESPACE