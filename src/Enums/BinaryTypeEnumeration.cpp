#include "BinaryTypeEnumeration.h"

#include <QDataStream>

QNRBF_BEGIN_NAMESPACE

bool Parser::readBinaryTypeEnum(BinaryTypeEnumeration &out, QDataStream &in) {
    in >> out;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE