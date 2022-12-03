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

QNRBF_END_NAMESPACE
