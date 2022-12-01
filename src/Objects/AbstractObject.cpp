#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

AbstractObject::AbstractObject(Type type) : t(type) {
    insts.insert(this);
}

AbstractObject::~AbstractObject() {
    insts.remove(this);
}

QNRBF_END_NAMESPACE
