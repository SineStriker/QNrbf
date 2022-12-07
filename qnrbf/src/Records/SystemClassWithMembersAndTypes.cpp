#include "SystemClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

SystemClassWithMembersAndTypes::SystemClassWithMembersAndTypes() {
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

bool QNrbf::SystemClassWithMembersAndTypes::write(QDataStream &out) const {
    if (!classInfo.write(out)) {
        return false;
    }
    if (!memberTypeInfo.write(out)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
