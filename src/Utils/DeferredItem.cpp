#include "DeferredItem.h"

QNRBF_BEGIN_NAMESPACE

DeferredItem::DeferredItem(DeferredItem::Action *action, qint32 id)
    : deferredAction(action), id(id) {
}

QNRBF_END_NAMESPACE
