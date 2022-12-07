#ifndef QNRBF_REMOTINGTYPEINFO_H
#define QNRBF_REMOTINGTYPEINFO_H

#include "Enums/BinaryTypeEnumeration.h"
#include "Enums/PrimitiveTypeEnumeration.h"
#include "Structures/ClassTypeInfo.h"

#include <QSharedDataPointer>
#include <QJsonValue>

QNRBF_BEGIN_NAMESPACE

class RemotingTypeInfoData;

class QNRBF_INTERNAL RemotingTypeInfo {
public:
    enum Type {
        None,
        PrimitiveType,
        String,
        Class,
    };

    RemotingTypeInfo();
    RemotingTypeInfo(const RemotingTypeInfo &other);
    RemotingTypeInfo &operator=(const RemotingTypeInfo &other);
    ~RemotingTypeInfo();

    RemotingTypeInfo(PrimitiveTypeEnumeration pte);
    RemotingTypeInfo(const QString &str);
    RemotingTypeInfo(const ClassTypeInfo &cti);

    bool isValid() const;
    Type type() const;

    PrimitiveTypeEnumeration toPrimitiveTypeEnum() const;
    QString toString() const;
    ClassTypeInfo toClassTypeInfo() const;

    QJsonValue readableTypeInfo() const;

    bool read(QDataStream &in, BinaryTypeEnumeration binaryTypeEnum);
    bool write(QDataStream &out) const;

protected:
    QSharedDataPointer<RemotingTypeInfoData> d;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_REMOTINGTYPEINFO_H
