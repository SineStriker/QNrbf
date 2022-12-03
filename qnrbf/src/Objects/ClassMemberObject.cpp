#include "ClassMemberObject.h"

QNRBF_BEGIN_NAMESPACE

ClassMemberObject::ClassMemberObject(QNrbf::ClassMemberObject::ClassType t)
    : AbstractObject(ClassMember), ct(t) {
    libraryId = 0;
}

QNRBF_END_NAMESPACE
