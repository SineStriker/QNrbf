#ifndef QNRBF_ABSTRACTOBJECT_H
#define QNRBF_ABSTRACTOBJECT_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API AbstractObject {
public:
    virtual ~AbstractObject();
};

QNRBF_END_NAMESPACE

#endif // QNRBF_ABSTRACTOBJECT_H
