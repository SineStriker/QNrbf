#include "PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

bool Parser::readPrimitiveTypeEnum(PrimitiveTypeEnumeration &out, QDataStream &in) {
    quint8 primitiveTypeEnum;
    in >> primitiveTypeEnum; // set as 0 if failed
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (primitiveTypeEnum == 0 || primitiveTypeEnum > (quint8) PrimitiveTypeEnumeration::String) {
        in.setStatus(QDataStream::ReadCorruptData);
        return false;
    }
    out = (PrimitiveTypeEnumeration) primitiveTypeEnum;
    return true;
}

QNRBF_END_NAMESPACE