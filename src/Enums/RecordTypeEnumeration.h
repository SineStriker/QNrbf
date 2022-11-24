#ifndef QNRBF_RECORDTYPEENUMERATION_H
#define QNRBF_RECORDTYPEENUMERATION_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.2.1 RecordTypeEnumeration

enum class RecordTypeEnumeration : quint8 {
    SerializedStreamHeader = 0,
    ClassWithId = 1,
    SystemClassWithMembers = 2,
    ClassWithMembers = 3,
    SystemClassWithMembersAndTypes = 4,
    ClassWithMembersAndTypes = 5,
    BinaryObjectString = 6,
    BinaryArray = 7,
    MemberPrimitiveTyped = 8,
    MemberReference = 9,
    ObjectNull = 10,
    MessageEnd = 11,
    BinaryLibrary = 12,
    ObjectNullMultiple256 = 13,
    ObjectNullMultiple = 14,
    ArraySinglePrimitive = 15,
    ArraySingleObject = 16,
    ArraySingleString = 17,
    MethodCall = 21,
    MethodReturn = 22,
};

namespace Parser {

    QNRBF_API bool readRecordTypeEnum(RecordTypeEnumeration &out, QDataStream &in);

};

QNRBF_END_NAMESPACE

Q_DECLARE_METATYPE(QNrbf::RecordTypeEnumeration);

#endif // QNRBF_RECORDTYPEENUMERATION_H
