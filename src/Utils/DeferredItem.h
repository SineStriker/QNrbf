#ifndef QNRBFFORMAT_DEFERREDITEM_H
#define QNRBFFORMAT_DEFERREDITEM_H

#include "Objects/MappingObject.h"

#include <functional>

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL DeferredItem {
public:
    struct Action {
        virtual void Invoke(const ObjectRef &obj) = 0;
    };

    QSharedPointer<Action> deferredAction;
    qint32 id;

    DeferredItem(Action *action, qint32 id);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_DEFERREDITEM_H
