#ifndef QNRBFFORMAT_SYSTEMCLASSOBJECT_H
#define QNRBFFORMAT_SYSTEMCLASSOBJECT_H

#include "ClassMemberObject.h"
#include "Records/SystemClassWithMembers.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL SystemClassObject : public ClassMemberObject {
public:
    SystemClassObject();
    SystemClassObject(const SystemClassWithMembers &record);
    SystemClassObject(const SystemClassWithMembers &record, const MappingRef &value);

    void fromRecord(const SystemClassWithMembers &record);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SYSTEMCLASSOBJECT_H
