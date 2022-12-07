#include "RecordTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

QString Parser::strRecordTypeEnum(RecordTypeEnumeration type) {
    QString res;
    switch (type) {
        case RecordTypeEnumeration::SerializedStreamHeader:
            res = "SerializedStreamHeader";
            break;
        case RecordTypeEnumeration::ClassWithId:
            res = "ClassWithId";
            break;
        case RecordTypeEnumeration::SystemClassWithMembers:
            res = "SystemClassWithMembers";
            break;
        case RecordTypeEnumeration::ClassWithMembers:
            res = "ClassWithMembers";
            break;
        case RecordTypeEnumeration::SystemClassWithMembersAndTypes:
            res = "SystemClassWithMembersAndTypes";
            break;
        case RecordTypeEnumeration::ClassWithMembersAndTypes:
            res = "ClassWithMembersAndTypes";
            break;
        case RecordTypeEnumeration::BinaryObjectString:
            res = "BinaryObjectString";
            break;
        case RecordTypeEnumeration::BinaryArray:
            res = "BinaryArray";
            break;
        case RecordTypeEnumeration::MemberPrimitiveTyped:
            res = "MemberPrimitiveTyped";
            break;
        case RecordTypeEnumeration::MemberReference:
            res = "MemberReference";
            break;
        case RecordTypeEnumeration::ObjectNull:
            res = "ObjectNull";
            break;
        case RecordTypeEnumeration::MessageEnd:
            res = "MessageEnd";
            break;
        case RecordTypeEnumeration::BinaryLibrary:
            res = "BinaryLibrary";
            break;
        case RecordTypeEnumeration::ObjectNullMultiple256:
            res = "ObjectNullMultiple256";
            break;
        case RecordTypeEnumeration::ObjectNullMultiple:
            res = "ObjectNullMultiple";
            break;
        case RecordTypeEnumeration::ArraySinglePrimitive:
            res = "ArraySinglePrimitive";
            break;
        case RecordTypeEnumeration::ArraySingleObject:
            res = "ArraySingleObject";
            break;
        case RecordTypeEnumeration::ArraySingleString:
            res = "ArraySingleString";
            break;
        case RecordTypeEnumeration::MethodCall:
            res = "MethodCall";
            break;
        case RecordTypeEnumeration::MethodReturn:
            res = "MethodReturn";
            break;
        default:
            res = QString("Unknown Type %1").arg(qint32(type));
            break;
    }
    return res;
}

bool Parser::readRecordTypeEnum(RecordTypeEnumeration &out, QDataStream &in) {
    quint8 recordTypeEnum;
    in >> recordTypeEnum; // set as 0 if failed
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (recordTypeEnum <= (quint8) RecordTypeEnumeration::ArraySingleString ||
        (recordTypeEnum >= (quint8) RecordTypeEnumeration::MethodCall &&
         recordTypeEnum <= (quint8) RecordTypeEnumeration::MethodReturn)) {
        out = (RecordTypeEnumeration) recordTypeEnum;
        return true;
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
        return false;
    }
}

bool Parser::writeRecordTypeEnum(RecordTypeEnumeration in, QDataStream &out) {
    out << quint8(in);
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
