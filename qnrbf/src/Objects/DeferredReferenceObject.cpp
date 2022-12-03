#include "DeferredReferenceObject.h"

QNRBF_BEGIN_NAMESPACE

DeferredReferenceObject::DeferredReferenceObject(qint32 id)
    : AbstractObject(DeferredReference), id(id) {
}

QNRBF_END_NAMESPACE