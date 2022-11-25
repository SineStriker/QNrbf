#ifndef QNRBF_BINARYOBJECT_H
#define QNRBF_BINARYOBJECT_H

#include "AbstractObject.h"

QNRBF_BEGIN_NAMESPACE

class BinaryObject : public AbstractObject {
public:
    BinaryObject();

    QString typeName;
    QString assemblyName;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYOBJECT_H
