#include "SystemClassTypeObject.h"

QNRBF_BEGIN_NAMESPACE

SystemClassTypeObject::SystemClassTypeObject() : ClassMemberObject(SystemWithTypes) {
}

SystemClassTypeObject::SystemClassTypeObject(const SystemClassWithMembersAndTypes &record)
    : SystemClassTypeObject() {
    fromRecord(record);
}

void SystemClassTypeObject::fromRecord(const SystemClassWithMembersAndTypes &record) {
    classInfo = record.classInfo;
    memberTypeInfo = record.memberTypeInfo;
}

QNRBF_END_NAMESPACE