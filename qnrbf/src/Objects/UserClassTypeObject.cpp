#include "UserClassTypeObject.h"

QNRBF_BEGIN_NAMESPACE

UserClassTypeObject::UserClassTypeObject() : ClassMemberObject(UserWithTypes) {
}

UserClassTypeObject::UserClassTypeObject(const ClassWithMembersAndTypes &record)
    : UserClassTypeObject() {
    fromRecord(record);
}

UserClassTypeObject::UserClassTypeObject(const ClassWithMembersAndTypes &record,
                                         const MappingRef &value)
    : UserClassTypeObject(record) {
    this->value = value;
}

void UserClassTypeObject::fromRecord(const ClassWithMembersAndTypes &record) {
    classInfo = record.classInfo;
    memberTypeInfo = record.memberTypeInfo;
    libraryId = record.libraryId;
}

QNRBF_END_NAMESPACE
