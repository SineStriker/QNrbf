#ifndef QNRBF_REMOTINGTYPEINFO_H
#define QNRBF_REMOTINGTYPEINFO_H

#include <QVariant>

#include "Enums/BinaryTypeEnumeration.h"
#include "Enums/PrimitiveTypeEnumeration.h"
#include "Structures/ClassTypeInfo.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API RemotingTypeInfo {
public:
    PrimitiveTypeEnumeration toPrimitiveTypeEnum() const;
    QString toString() const;
    ClassTypeInfo toClassTypeInfo() const;

    QVariant _data;

    bool read(QDataStream &in, BinaryTypeEnumeration binaryTypeEnum);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_REMOTINGTYPEINFO_H
