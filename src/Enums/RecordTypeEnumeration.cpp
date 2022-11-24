#include "RecordTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

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

QNRBF_END_NAMESPACE
