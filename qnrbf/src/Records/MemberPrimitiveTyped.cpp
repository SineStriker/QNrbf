#include "MemberPrimitiveTyped.h"

QNRBF_BEGIN_NAMESPACE

MemberPrimitiveTyped::MemberPrimitiveTyped() {
}

bool MemberPrimitiveTyped::read(QDataStream &in) {
    if (!value.read(in)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
