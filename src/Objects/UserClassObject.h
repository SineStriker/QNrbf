#ifndef QNRBFFORMAT_USERCLASSOBJECT_H
#define QNRBFFORMAT_USERCLASSOBJECT_H

#include "ClassMemberObject.h"
#include "Records/ClassWithMembers.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API UserClassObject : public ClassMemberObject {
public:
    UserClassObject();
    UserClassObject(const ClassWithMembers &record);
    UserClassObject(const ClassWithMembers &record, const BinaryObjectRef &value);

    void fromRecord(const ClassWithMembers &record);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_USERCLASSOBJECT_H
