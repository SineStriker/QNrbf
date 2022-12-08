#include "AbstractObject.h"

#include <QDebug>

QNRBF_BEGIN_NAMESPACE

QSet<AbstractObject *> AbstractObject::insts{};

AbstractObject::AbstractObject(Type type) : t(type), id(0) {
    insts.insert(this);
}

AbstractObject::~AbstractObject() {
    insts.remove(this);
}

QNRBF_END_NAMESPACE
