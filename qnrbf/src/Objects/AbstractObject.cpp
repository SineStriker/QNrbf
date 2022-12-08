#include "AbstractObject.h"

#include <QDebug>

QNRBF_BEGIN_NAMESPACE

#ifndef DISABLE_DEBUG_OBJECT_COLLECT
QSet<AbstractObject *> AbstractObject::insts{};
#endif

AbstractObject::AbstractObject(Type type) : t(type), id(0) {
#ifndef DISABLE_DEBUG_OBJECT_COLLECT
    insts.insert(this);
#endif
}

AbstractObject::~AbstractObject() {
#ifndef DISABLE_DEBUG_OBJECT_COLLECT
    insts.remove(this);
#endif
}

QNRBF_END_NAMESPACE
