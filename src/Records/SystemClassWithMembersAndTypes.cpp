#include "SystemClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

bool SystemClassWithMembersAndTypes::read(QDataStream &in) {
    if (!classInfo.read(in)) {
        return false;
    }
    if (!memberTypeInfo.read(in)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
