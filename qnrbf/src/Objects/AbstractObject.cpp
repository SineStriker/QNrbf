#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

AbstractObject::AbstractObject(Type type) : t(type), id(0) {
    insts.insert(this);
}

AbstractObject::~AbstractObject() {
    insts.remove(this);
}

QNRBF_END_NAMESPACE
