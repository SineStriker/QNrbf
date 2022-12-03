#ifndef QNRBFFORMAT_DEFERREDREFERENCEOBJECT_H
#define QNRBFFORMAT_DEFERREDREFERENCEOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL DeferredReferenceObject : public AbstractObject {
public:
    DeferredReferenceObject(qint32 id);

    int id;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_DEFERREDREFERENCEOBJECT_H
