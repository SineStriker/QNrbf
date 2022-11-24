#include "ArraySingleString.h"

QNRBF_BEGIN_NAMESPACE

bool QNrbf::ArraySingleString::read(QDataStream &in) {
    return arrayInfo.read(in);
}

QNRBF_END_NAMESPACE