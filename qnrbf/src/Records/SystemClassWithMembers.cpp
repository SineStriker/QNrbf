#include "SystemClassWithMembers.h"

QNRBF_BEGIN_NAMESPACE

SystemClassWithMembers::SystemClassWithMembers() {
}

bool SystemClassWithMembers::read(QDataStream &in) {
    return classInfo.read(in);
}

bool SystemClassWithMembers::write(QDataStream &out) const {
    return classInfo.write(out);
}

QNRBF_END_NAMESPACE
