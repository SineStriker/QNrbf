#include "ClassWithMembers.h"

QNRBF_BEGIN_NAMESPACE

ClassWithMembers::ClassWithMembers() {
    libraryId = 0;
}

bool ClassWithMembers::read(QDataStream &in) {
    if (!classInfo.read(in)) {
        return false;
    }
    in >> libraryId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
