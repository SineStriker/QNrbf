#ifndef QNRBFFORMAT_ONEORMORENULLOBJECT_H
#define QNRBFFORMAT_ONEORMORENULLOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API OneOrMoreNullObject : public AbstractObject {
public:
    OneOrMoreNullObject();
    explicit OneOrMoreNullObject(qint32 nullCount);

    qint32 nullCount;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_ONEORMORENULLOBJECT_H
