#include "ReadHelper.h"

#include "Objects/DataObject.h"
#include "Objects/DeferredReferenceObject.h"
#include "Objects/MemberPrimitiveObject.h"
#include "Objects/OneOrMoreNullObject.h"
#include "Objects/SystemClassObject.h"
#include "Objects/SystemClassTypeObject.h"
#include "Objects/UserClassObject.h"
#include "Objects/UserClassTypeObject.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::ReadHelper::ReadHelper(QDataStream *stream) : stream(stream) {
    Q_ASSERT(stream);
}

ReadHelper::~ReadHelper() {
}

bool ReadHelper::onClassWithId(ClassWithId &in, ObjectRef &out) {
    auto classRef = dynamic_cast<ClassMemberObject *>(objects[in.metadataId].data());
    if (!classRef) {
        return false;
    }
    auto objRef = classRef->value.data();

    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = objRef->typeName;
    obj->assemblyName = objRef->assemblyName;

    if (in.objectId != 0) {
        objects[in.objectId] = obj;
    }
    out = obj;

    if (classRef->classType() & ClassMemberObject::WithTypes) {
        if (!readMembers(*obj, classRef->classInfo.memberNames, classRef->memberTypeInfo)) {
            return false;
        }
    } else {
        if (!readUntypedMembers(*obj, obj->typeName, classRef->classInfo.memberNames)) {
            return false;
        }
    }

    return true;
}

bool ReadHelper::onSystemClassWithMembers(SystemClassWithMembers &in, ObjectRef &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = ObjectRef(new SystemClassObject(in, obj));
    }

    out = obj;

    if (!readUntypedMembers(*obj, in.classInfo.name, in.classInfo.memberNames)) {
        return false;
    }

    return true;
}
bool ReadHelper::onClassWithMembers(ClassWithMembers &in, ObjectRef &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = libraries[in.libraryId];

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = ObjectRef(new UserClassObject(in, obj));
    }

    out = obj;

    if (!readUntypedMembers(*obj, in.classInfo.name, in.classInfo.memberNames)) {
        return false;
    }

    return true;
}
bool ReadHelper::onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in,
                                                  ObjectRef &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = ObjectRef(new SystemClassTypeObject(in, obj));
    }

    out = obj;

    if (!readMembers(*obj, in.classInfo.memberNames, in.memberTypeInfo)) {
        return false;
    }

    return true;
}
bool ReadHelper::onClassWithMembersAndTypes(ClassWithMembersAndTypes &in, ObjectRef &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = libraries[in.libraryId];

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = ObjectRef(new UserClassTypeObject(in, obj));
    }

    out = obj;

    if (!readMembers(*obj, in.classInfo.memberNames, in.memberTypeInfo)) {
        return false;
    }

    return true;
}

bool ReadHelper::onBinaryObjectString(BinaryObjectString &in, ObjectRef &out) {

    if (in.objectId != 0) {
        objects[in.objectId] = ObjectRef(new DataObject(in.value));
    }

    return true;
}

bool ReadHelper::onBinaryArray(BinaryArray &in, ObjectRef &out) {
    return true;
}

bool ReadHelper::onMemberPrimitiveTyped(MemberPrimitiveTyped &in, ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new MemberPrimitiveObject(in));
    return true;
}

bool ReadHelper::onMemberReference(MemberReference &in, ObjectRef &out) {
    auto it = objects.find(in.idRef);
    if (it != objects.end()) {
        // Use cached one
        auto &objectRef = it.value();
        if (objectRef->type() == AbstractObject::ClassMember) {
            out = dynamic_cast<ClassMemberObject *>(objectRef.data())->value;
        } else {
            out = objectRef;
        }
    } else {
        // Allocate a defer request
        out = ObjectRef(new DeferredReferenceObject(in.idRef));
    }
    return true;
}

bool ReadHelper::onBinaryLibrary(BinaryLibrary &in, ObjectRef &out) {
    libraries[in.libraryId] = in.libraryName;
    return false;
}

bool ReadHelper::onObjectNull(ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new OneOrMoreNullObject(1));
    return true;
}

bool ReadHelper::onObjectNullMultiple(ObjectNullMultiple &in, ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new OneOrMoreNullObject(in.nullCount));
    return true;
}

bool ReadHelper::onArraySinglePrimitive(ArraySinglePrimitive &in, ObjectRef &out) {
    return false;
}

bool ReadHelper::onArraySingleObject(ArraySingleObject &in, ObjectRef &out) {
    return false;
}

bool ReadHelper::onArraySingleString(ArraySingleString &in, ObjectRef &out) {
    return false;
}

bool ReadHelper::readMembers(BinaryObject &acceptor, const QStringList &memberNames,
                             const MemberTypeInfo &memberTypeInfo) {
    return false;
}

bool ReadHelper::readUntypedMembers(BinaryObject &acceptor, const QString &className,
                                    const QStringList &memberNames) {
    return false;
}

QNRBF_END_NAMESPACE