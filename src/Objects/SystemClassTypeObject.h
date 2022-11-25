#ifndef QNRBFFORMAT_SYSTEMCLASSTYPEOBJECT_H
#define QNRBFFORMAT_SYSTEMCLASSTYPEOBJECT_H

#include "ClassMemberObject.h"
#include "Records//SystemClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL SystemClassTypeObject : public ClassMemberObject {
public:
    SystemClassTypeObject();
    SystemClassTypeObject(const SystemClassWithMembersAndTypes &record);
    SystemClassTypeObject(const SystemClassWithMembersAndTypes &record,
                          const BinaryObjectRef &value);

    void fromRecord(const SystemClassWithMembersAndTypes &record);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_SYSTEMCLASSTYPEOBJECT_H
