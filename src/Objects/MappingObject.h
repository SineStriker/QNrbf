#ifndef QNRBF_BINARYOBJECT_H
#define QNRBF_BINARYOBJECT_H

#include "AbstractObject.h"

#include <QMap>

QNRBF_BEGIN_NAMESPACE

class MappingObject : public AbstractObject {
public:
    MappingObject();

    QString typeName;
    QString assemblyName;

    QMap<QString, ObjectRef> members;
};

using BinaryObjectRef = QSharedPointer<MappingObject>;

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYOBJECT_H
