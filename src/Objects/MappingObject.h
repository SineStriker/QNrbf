#ifndef QNRBF_BINARYOBJECT_H
#define QNRBF_BINARYOBJECT_H

#include "AbstractObject.h"

#include <QMap>

QNRBF_BEGIN_NAMESPACE

class BinaryObject : public AbstractObject {
public:
    BinaryObject();

    QString typeName;
    QString assemblyName;

    QMap<QString, ObjectRef> members;
};

using BinaryObjectRef = QSharedPointer<BinaryObject>;

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYOBJECT_H
