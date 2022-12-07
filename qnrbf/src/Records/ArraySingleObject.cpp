#include "ArraySingleObject.h"

QNRBF_BEGIN_NAMESPACE

bool ArraySingleObject::read(QDataStream &in) {
    return arrayInfo.read(in);
}

bool ArraySingleObject::write(QDataStream &out) {
    return arrayInfo.write(out);
}

QNRBF_END_NAMESPACE
