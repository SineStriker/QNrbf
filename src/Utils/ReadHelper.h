#ifndef QNRBF_READHELPER_H
#define QNRBF_READHELPER_H

// Private part

#include "Objects/BinaryObject.h"

#include "Records/ArraySingleObject.h"
#include "Records/ArraySinglePrimitive.h"
#include "Records/ArraySingleString.h"
#include "Records/BinaryArray.h"
#include "Records/BinaryLibrary.h"
#include "Records/BinaryObjectString.h"
#include "Records/ClassWithId.h"
#include "Records/ClassWithMembers.h"
#include "Records/ClassWithMembersAndTypes.h"
#include "Records/MemberPrimitiveTyped.h"
#include "Records/MemberReference.h"
#include "Records/ObjectNullMultiple.h"
#include "Records/SystemClassWithMembers.h"
#include "Records/SystemClassWithMembersAndTypes.h"

#include "Enums/RecordTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL ReadHelper {
public:
    explicit ReadHelper(QDataStream *stream);
    ~ReadHelper();

    enum Status {
        Normal = 1,
        ReachEnd = 3,
        Failed = 4,
        MultipleHead = 6,
        UnsupportedRecord = 8,
    };

    Status status() const;

    bool read();

    void reset();

protected:
    bool readRecord(BinaryObjectRef &out);

    bool readMembers(BinaryObject &acceptor, const QStringList &memberNames,
                     const MemberTypeInfo &memberTypeInfo);

    bool readUntypedMembers(BinaryObject &acceptor, const QString &className,
                            const QStringList &memberNames);

    bool onClassWithId(ClassWithId &in, ObjectRef &out);

    bool onSystemClassWithMembers(SystemClassWithMembers &in, ObjectRef &out);

    bool onClassWithMembers(ClassWithMembers &in, ObjectRef &out);

    bool onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in, ObjectRef &out);

    bool onClassWithMembersAndTypes(ClassWithMembersAndTypes &in, ObjectRef &out);

    bool onBinaryObjectString(BinaryObjectString &in, ObjectRef &out);

    bool onBinaryArray(BinaryArray &in, ObjectRef &out);

    bool onMemberPrimitiveTyped(MemberPrimitiveTyped &in, ObjectRef &out);

    bool onMemberReference(MemberReference &in, ObjectRef &out);

    bool onBinaryLibrary(BinaryLibrary &in, ObjectRef &out);

    bool onObjectNull(ObjectRef &out);

    bool onObjectNullMultiple(ObjectNullMultiple &in, ObjectRef &out);

    bool onArraySinglePrimitive(ArraySinglePrimitive &in, ObjectRef &out);

    bool onArraySingleObject(ArraySingleObject &in, ObjectRef &out);

    bool onArraySingleString(ArraySingleString &in, ObjectRef &out);

    // Caches
    QHash<qint32, ObjectRef> objects;

    QHash<qint32, QString> libraries;

    bool hasHead;

    // Properties
    QDataStream *stream;

    Status _status;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_READHELPER_H
