#ifndef QNRBF_MEMBERTYPEINFO_H
#define QNRBF_MEMBERTYPEINFO_H

#include "Common/RemotingTypeInfo.h"
#include "Enums/BinaryTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.1.2 MemberTypeInfo

class QNRBF_API MemberTypeInfo {
public:
    QList<BinaryTypeEnumeration> binaryTypeEnums;

    QList<RemotingTypeInfo> additionalInfos;

    bool read(QDataStream &in, qint32 memberCount);
};

QNRBF_END_NAMESPACE

Q_DECLARE_METATYPE(QNrbf::MemberTypeInfo);

#endif // QNRBF_MEMBERTYPEINFO_H
