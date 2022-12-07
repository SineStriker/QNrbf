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

bool ClassWithMembers::write(QDataStream &out) const {
    if (!classInfo.write(out)) {
        return false;
    }
    out << libraryId;
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
