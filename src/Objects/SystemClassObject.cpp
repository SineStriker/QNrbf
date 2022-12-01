#include "SystemClassObject.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::SystemClassObject::SystemClassObject() : ClassMemberObject(System) {
}

SystemClassObject::SystemClassObject(const SystemClassWithMembers &record) : SystemClassObject() {
    fromRecord(record);
}

SystemClassObject::SystemClassObject(const SystemClassWithMembers &record,
                                     const MappingRef &value)
    : SystemClassObject(record) {
    this->value = value;
}

void SystemClassObject::fromRecord(const SystemClassWithMembers &record) {
    classInfo = record.classInfo;
}

QNRBF_END_NAMESPACE