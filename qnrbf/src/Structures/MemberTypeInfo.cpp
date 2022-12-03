#include "MemberTypeInfo.h"

QNRBF_BEGIN_NAMESPACE

bool MemberTypeInfo::read(QDataStream &in, qint32 memberCount) {
    binaryTypeEnums.reserve(memberCount);
    additionalInfos.reserve(memberCount);

    // Read enums
    for (qint32 i = 0; i < memberCount; ++i) {
        QNrbf::BinaryTypeEnumeration binaryTypeEnum;
        if (!Parser::readBinaryTypeEnum(binaryTypeEnum, in)) {
            return false;
        }
        binaryTypeEnums.append(binaryTypeEnum);
    }

    // Read additional infos
    for (qint32 i = 0; i < memberCount; ++i) {
        QNrbf::RemotingTypeInfo info;
        if (!info.read(in, binaryTypeEnums.at(i))) {
            return false;
        }
        additionalInfos.append(info);
    }

    return true;
}

QNRBF_END_NAMESPACE
