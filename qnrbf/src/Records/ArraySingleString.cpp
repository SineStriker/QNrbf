#include "ArraySingleString.h"

QNRBF_BEGIN_NAMESPACE

bool QNrbf::ArraySingleString::read(QDataStream &in) {
    return arrayInfo.read(in);
}

bool QNrbf::ArraySingleString::write(QDataStream &out) const {
    return arrayInfo.write(out);
}

QNRBF_END_NAMESPACE