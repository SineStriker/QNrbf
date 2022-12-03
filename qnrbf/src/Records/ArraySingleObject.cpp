#include "ArraySingleObject.h"

QNRBF_BEGIN_NAMESPACE

bool ArraySingleObject::read(QDataStream &in) {
    return arrayInfo.read(in);
}

QNRBF_END_NAMESPACE