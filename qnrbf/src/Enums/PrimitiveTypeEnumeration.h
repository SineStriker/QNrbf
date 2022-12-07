#ifndef QNRBF_PRIMITIVETYPEENUMERATION_H
#define QNRBF_PRIMITIVETYPEENUMERATION_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.2.3 PrimitiveTypeEnumeration

enum class PrimitiveTypeEnumeration : quint8 {
    None = 0,
    Boolean = 1,
    Byte = 2,
    Char = 3,
    Unused = 4,
    Decimal = 5,
    Double = 6,
    Int16 = 7,
    Int32 = 8,
    Int64 = 9,
    SByte = 10,
    Single = 11,
    TimeSpan = 12,
    DateTime = 13,
    UInt16 = 14,
    UInt32 = 15,
    UInt64 = 16,
    Null = 17,
    String = 18,
};

namespace Parser {

    QNRBF_INTERNAL QString strPrimitiveTypeEnum(PrimitiveTypeEnumeration type);

    QNRBF_INTERNAL bool readPrimitiveTypeEnum(PrimitiveTypeEnumeration &out, QDataStream &in);

    QNRBF_INTERNAL bool writePrimitiveTypeEnum(PrimitiveTypeEnumeration in, QDataStream &out);

}; // namespace Parser

QNRBF_END_NAMESPACE

#endif // QNRBF_PRIMITIVETYPEENUMERATION_H
