#ifndef QNRBF_BINARYTYPEENUMERATION_H
#define QNRBF_BINARYTYPEENUMERATION_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.2.2 BinaryTypeEnumeration

enum class BinaryTypeEnumeration : quint8 {
    Primitive = 0,
    String = 1,
    Object = 2,
    SystemClass = 3,
    Class = 4,
    ObjectArray = 5,
    StringArray = 6,
    PrimitiveArray = 7,
};

namespace Parser {

    QNRBF_API bool readBinaryTypeEnum(BinaryTypeEnumeration &out, QDataStream &in);

};

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYTYPEENUMERATION_H
