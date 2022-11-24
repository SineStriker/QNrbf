#include "ClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::ClassWithMembersAndTypes::ClassWithMembersAndTypes() : ClassMemberObject(UserWithTypes) {
    libraryId = 0;
}

bool ClassWithMembersAndTypes::read(QDataStream &in) {
    if (!classInfo.read(in)) {
        return false;
    }
    if (!memberTypeInfo.read(in, classInfo.memberCount)) {
        return false;
    }
    in >> libraryId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE