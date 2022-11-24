#include "MemberReference.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::MemberReference::MemberReference() {
    idRef = 0;
}

bool MemberReference::read(QDataStream &in) {
    return false;
}

QNRBF_END_NAMESPACE
