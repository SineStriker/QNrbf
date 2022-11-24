#include "BinaryArrayTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

bool Parser::readBinaryArrayTypeEnum(QNrbf::BinaryArrayTypeEnumeration &out, QDataStream &in) {
    quint8 binArrayTypeEnum;
    in >> binArrayTypeEnum;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (binArrayTypeEnum > (quint8) QNrbf::BinaryArrayTypeEnumeration::RectangularOffset) {
        in.setStatus(QDataStream::ReadCorruptData);
        return false;
    }
    out = (QNrbf::BinaryArrayTypeEnumeration) binArrayTypeEnum;
    return true;
}

QNRBF_END_NAMESPACE
