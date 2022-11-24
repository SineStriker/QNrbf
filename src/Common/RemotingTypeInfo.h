#ifndef QNRBF_REMOTINGTYPEINFO_H
#define QNRBF_REMOTINGTYPEINFO_H

#include <QVariant>

#include "Enums/PrimitiveTypeEnumeration.h"
#include "Structures/ClassTypeInfo.h"

QNRBF_BEGIN_NAMESPACE

class RemotingTypeInfo {
public:
    PrimitiveTypeEnumeration toPrimitiveTypeEnum() const;
    QString toString() const;
    ClassTypeInfo toClassTypeInfo() const;

    QVariant _data;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_REMOTINGTYPEINFO_H
