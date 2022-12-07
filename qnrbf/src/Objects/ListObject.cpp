#include "ListObject.h"

QNRBF_BEGIN_NAMESPACE

ListObject::ListObject(QNrbf::AbstractObject::Type type) : AbstractObject(type) {
    hasShapeInfo = false;
}

QNRBF_END_NAMESPACE