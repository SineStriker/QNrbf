#ifndef QNRBF_READHELPER_H
#define QNRBF_READHELPER_H

// Private part

#include "Objects/BinaryObject.h"

#include "Records/BinaryArray.h"
#include "Records/BinaryObjectString.h"
#include "Records/ClassWithId.h"
#include "Records/ClassWithMembers.h"
#include "Records/ClassWithMembersAndTypes.h"
#include "Records/SystemClassWithMembers.h"
#include "Records/SystemClassWithMembersAndTypes.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API ReadHelper {
public:
    explicit ReadHelper(QDataStream *stream);
    ~ReadHelper();

    bool onClassWithId(ClassWithId &in, ObjectRef &out);

    bool onSystemClassWithMembers(SystemClassWithMembers &in, ObjectRef &out);

    bool onClassWithMembers(ClassWithMembers &in, ObjectRef &out);

    bool onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in, ObjectRef &out);

    bool onClassWithMembersAndTypes(ClassWithMembersAndTypes &in, ObjectRef &out);

    bool onBinaryObjectString(BinaryObjectString &in, ObjectRef &out);

    bool onBinaryArray(BinaryArray &in, ObjectRef &out);

    QHash<qint32, ObjectRef> objects;
    QHash<qint32, QString> libraries;

protected:
    QDataStream *stream;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_READHELPER_H
