#ifndef QNRBF_NRBFREADER_H
#define QNRBF_NRBFREADER_H

// Private part

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
#include "Records/SerializationHeader.h"
#include "Records/SystemClassWithMembers.h"
#include "Records/SystemClassWithMembersAndTypes.h"

#include "Objects/ClassMemberObject.h"
#include "Objects/ObjectListObject.h"

#include "DeferredItem.h"
#include "Enums/RecordTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL NrbfReader {
public:
    explicit NrbfReader(QDataStream *stream);
    ~NrbfReader();

    enum Status {
        Normal = 1,
        ReachEnd = 3,
        Failed = 4,
        MultipleHead = 6,
        UnsupportedRecord = 8,
    };

    Status status() const;

    ObjectRef read();

    void reset();

protected:
    /* Read record of structure */
    bool readRecord(ObjectRef *out = nullptr);

    /* Read series of class members in stream */
    bool readMembers(const MappingRef &acceptor, const QStringList &memberNames,
                     const MemberTypeInfo &memberTypeInfo);

    bool readUntypedMembers(const MappingRef &acceptor, const QString &className,
                            const QStringList &memberNames);

    /* Subsequent work after reading a certain record */
    bool onSystemClassWithMembers(SystemClassWithMembers &in, ObjectRef &out);

    bool onClassWithMembers(ClassWithMembers &in, ObjectRef &out);

    bool onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in, ObjectRef &out);

    bool onClassWithMembersAndTypes(ClassWithMembersAndTypes &in, ObjectRef &out);

    bool onClassWithId(ClassWithId &in, ObjectRef &out);

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

    /* Reusable reading method */
    bool readStrings(QStringList &arr);

    bool readObjects(QList<ObjectRef> &arr, const QSharedPointer<ObjectListObject> &parent);

    /* Call after reaching message end, when all reference are collected */
    void resolveDeferredItems();

    ObjectRef findReference(qint32 id);

    // Defined references
    QHash<qint32, ObjectRef> objectsById;

    QHash<qint32, ClassRef> classesById;

    QSharedPointer<SerializationHeader> header;

    // Libraries
    QHash<qint32, QString> libraries;

    QList<DeferredItem> deferredItems;

    // Properties
    QDataStream *stream;

    Status _status;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_NRBFREADER_H
