#include "SystemClassWithMembers.h"

QNRBF_BEGIN_NAMESPACE

bool SystemClassWithMembers::read(QDataStream &in) {
    return classInfo.read(in);
}

QNRBF_END_NAMESPACE
