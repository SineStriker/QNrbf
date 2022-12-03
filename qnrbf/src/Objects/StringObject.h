#ifndef QNRBFFORMAT_STRINGOBJECT_H
#define QNRBFFORMAT_STRINGOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL StringObject : public AbstractObject {
public:
    explicit StringObject(const QString &val);

    QString value;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_STRINGOBJECT_H
