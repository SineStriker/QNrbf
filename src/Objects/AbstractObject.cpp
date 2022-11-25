#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

AbstractObject::AbstractObject(Type type) : t(type) {
}

AbstractObject::~AbstractObject() = default;

QNRBF_END_NAMESPACE
