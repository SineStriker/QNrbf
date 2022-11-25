#ifndef QNRBFFORMAT_USERCLASSTYPEOBJECT_H
#define QNRBFFORMAT_USERCLASSTYPEOBJECT_H

#include "ClassMemberObject.h"
#include "Records/ClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API UserClassTypeObject : public ClassMemberObject {
public:
    UserClassTypeObject();
    UserClassTypeObject(const ClassWithMembersAndTypes &record);
    UserClassTypeObject(const ClassWithMembersAndTypes &record, const BinaryObjectRef &value);

    void fromRecord(const ClassWithMembersAndTypes &record);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_USERCLASSTYPEOBJECT_H
