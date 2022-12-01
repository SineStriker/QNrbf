#include "MemberPrimitiveObject.h"

QNRBF_BEGIN_NAMESPACE

MemberPrimitiveObject::MemberPrimitiveObject(const PrimitiveValue &val)
    : AbstractObject(Primitive), value(val) {
}

QNRBF_END_NAMESPACE