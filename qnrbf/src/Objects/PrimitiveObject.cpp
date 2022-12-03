#include "PrimitiveObject.h"

QNRBF_BEGIN_NAMESPACE

PrimitiveObject::PrimitiveObject(const PrimitiveValue &val)
    : AbstractObject(Primitive), value(val) {
}

QNRBF_END_NAMESPACE