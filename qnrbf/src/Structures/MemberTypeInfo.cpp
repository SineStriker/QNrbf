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

bool MemberTypeInfo::write(QDataStream &out) const {
    // Write enums
    for (const auto &item : qAsConst(binaryTypeEnums)) {
        if (!Parser::writeBinaryTypeEnum(item, out)) {
            return false;
        }
    }

    // Read additional infos
    for (const auto &item : qAsConst(additionalInfos)) {
        if (!item.write(out)) {
            return false;
        }
    }
    return true;
}

QNRBF_END_NAMESPACE
