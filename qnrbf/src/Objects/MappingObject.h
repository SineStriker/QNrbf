#ifndef QNRBF_MAPPINGOBJECT_H
#define QNRBF_MAPPINGOBJECT_H

#include "AbstractObject.h"

#include <QMap>

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL MappingObject : public AbstractObject {
public:
    MappingObject();

    QString typeName;
    QString assemblyName;
    qint32 classId;

    QMap<QString, ObjectRef> members;

    // Debug usage
    QString parentInfo;
};

using MappingRef = QSharedPointer<MappingObject>;

QNRBF_END_NAMESPACE

#endif // QNRBF_MAPPINGOBJECT_H
