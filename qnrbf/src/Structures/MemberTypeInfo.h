#ifndef QNRBF_MEMBERTYPEINFO_H
#define QNRBF_MEMBERTYPEINFO_H

#include "Common/RemotingTypeInfo.h"
#include "Enums/BinaryTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.1.2 MemberTypeInfo

class QNRBF_INTERNAL MemberTypeInfo {
public:
    QList<BinaryTypeEnumeration> binaryTypeEnums;

    QList<RemotingTypeInfo> additionalInfos;

    bool read(QDataStream &in, qint32 memberCount);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_MEMBERTYPEINFO_H
