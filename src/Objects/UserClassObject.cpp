#include "UserClassObject.h"

QNRBF_BEGIN_NAMESPACE

UserClassObject::UserClassObject() : ClassMemberObject(User) {
}

UserClassObject::UserClassObject(const ClassWithMembers &record) : UserClassObject() {
    fromRecord(record);
}

UserClassObject::UserClassObject(const ClassWithMembers &record, const MappingRef &value)
    : UserClassObject(record) {
    this->value = value;
}

void UserClassObject::fromRecord(const ClassWithMembers &record) {
    classInfo = record.classInfo;
    libraryId = record.libraryId;
}

QNRBF_END_NAMESPACE