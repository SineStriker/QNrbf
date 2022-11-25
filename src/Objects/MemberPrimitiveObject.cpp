#include "MemberPrimitiveObject.h"

QNRBF_BEGIN_NAMESPACE

MemberPrimitiveObject::MemberPrimitiveObject() {
    primitiveTypeEnum = PrimitiveTypeEnumeration::Null;
}

MemberPrimitiveObject::MemberPrimitiveObject(const QNrbf::MemberPrimitiveTyped &record)
    : MemberPrimitiveObject() {
    fromRecord(record);
}

void MemberPrimitiveObject::fromRecord(const MemberPrimitiveTyped &record) {
    this->primitiveTypeEnum = record.primitiveTypeEnum;
}

QNRBF_END_NAMESPACE