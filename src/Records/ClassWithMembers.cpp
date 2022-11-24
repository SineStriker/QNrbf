#include "ClassWithMembers.h"

QNRBF_BEGIN_NAMESPACE

ClassWithMembers::ClassWithMembers() {
    libraryId = 0;
}

bool ClassWithMembers::read(QDataStream &in) {
    return false;
}

QNRBF_END_NAMESPACE
