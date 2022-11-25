#ifndef QNRBFFORMAT_SYSTEMCLASSOBJECT_H
#define QNRBFFORMAT_SYSTEMCLASSOBJECT_H

#include "ClassMemberObject.h"
#include "Records/SystemClassWithMembers.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API SystemClassObject : public ClassMemberObject {
public:
    SystemClassObject();
    SystemClassObject(const SystemClassWithMembers &record);

    void fromRecord(const SystemClassWithMembers &record);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SYSTEMCLASSOBJECT_H
