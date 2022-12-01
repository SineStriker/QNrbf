#ifndef QNRBF_BINARYARRAYTYPEENUMERATION_H
#define QNRBF_BINARYARRAYTYPEENUMERATION_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.4.1.1 BinaryArrayTypeEnumeration
enum class BinaryArrayTypeEnumeration : quint8 {
    Single = 0,
    Jagged = 1,
    Rectangular = 2,
    SingleOffset = 3,
    JaggedOffset = 4,
    RectangularOffset = 5,
};

namespace Parser {

    QNRBF_INTERNAL bool readBinaryArrayTypeEnum(QNrbf::BinaryArrayTypeEnumeration &out,
                                                QDataStream &in);

};

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYARRAYTYPEENUMERATION_H
