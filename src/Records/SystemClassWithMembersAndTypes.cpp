#include "SystemClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

SystemClassWithMembersAndTypes::SystemClassWithMembersAndTypes()
    : ClassMemberObject(SystemWithTypes) {
}

bool SystemClassWithMembersAndTypes::read(QDataStream &in) {
    if (!classInfo.read(in)) {
        return false;
    }
    if (!memberTypeInfo.read(in, classInfo.memberCount)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
