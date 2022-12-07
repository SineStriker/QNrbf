#include "DeferredReferenceObject.h"

QNRBF_BEGIN_NAMESPACE

DeferredReferenceObject::DeferredReferenceObject(qint32 id)
    : AbstractObject(DeferredReference), idRef(id) {
}

QNRBF_END_NAMESPACE