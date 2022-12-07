#include "MemberPrimitiveTyped.h"

QNRBF_BEGIN_NAMESPACE

MemberPrimitiveTyped::MemberPrimitiveTyped() {
}

bool MemberPrimitiveTyped::read(QDataStream &in) {
    return value.read(in);
}

bool MemberPrimitiveTyped::write(QDataStream &out) const {
    return value.write(out);
}

QNRBF_END_NAMESPACE
