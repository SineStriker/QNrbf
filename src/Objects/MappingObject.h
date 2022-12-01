#ifndef QNRBF_BINARYOBJECT_H
#define QNRBF_BINARYOBJECT_H

#include "AbstractObject.h"

#include <QMap>

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL MappingObject : public AbstractObject {
public:
    MappingObject();

    QString typeName;
    QString assemblyName;

    QMap<QString, ObjectRef> members;
};

using MappingRef = QSharedPointer<MappingObject>;

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYOBJECT_H
