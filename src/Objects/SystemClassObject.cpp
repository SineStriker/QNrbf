#include "SystemClassObject.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::SystemClassObject::SystemClassObject() : ClassMemberObject(System) {
}

SystemClassObject::SystemClassObject(const SystemClassWithMembers &record) : SystemClassObject() {
    fromRecord(record);
}

void SystemClassObject::fromRecord(const SystemClassWithMembers &record) {
    classInfo = record.classInfo;
}

QNRBF_END_NAMESPACE