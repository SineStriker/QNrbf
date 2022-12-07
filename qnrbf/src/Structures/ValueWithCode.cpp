#include "ValueWithCode.h"

QNRBF_BEGIN_NAMESPACE

ValueWithCode::ValueWithCode() {
}

bool ValueWithCode::read(QDataStream &in) {
    if (!value.read(in)) {
        return false;
    }
    return true;
}

bool QNrbf::ValueWithCode::write(QDataStream &out) const {
    return value.write(out);
}

QNRBF_END_NAMESPACE
