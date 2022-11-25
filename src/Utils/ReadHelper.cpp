#include "ReadHelper.h"

#include "Objects/SystemClassObject.h"
#include "Objects/SystemClassTypeObject.h"
#include "Objects/UserClassObject.h"
#include "Objects/UserClassTypeObject.h"

#include "Objects/DataObject.h"

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

    if (classRef->type() & ClassMemberObject::WithTypes) {
        // ReadUntypedMembers
    } else {
        // ReadMembers
    }

    return true;
}

bool ReadHelper::onSystemClassWithMembers(SystemClassWithMembers &in, ObjectRef &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = ObjectRef(new SystemClassObject(in));
    }

    out = obj;

    // ReadUntypedMembers

    return true;
}
bool ReadHelper::onClassWithMembers(ClassWithMembers &in, ObjectRef &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = libraries[in.libraryId];

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = ObjectRef(new UserClassObject(in));
    }

    out = obj;

    // ReadUntypedMembers

    return true;
}
bool ReadHelper::onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in,
                                                  ObjectRef &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = ObjectRef(new SystemClassTypeObject(in));
    }

    out = obj;

    // ReadMembers

    return true;
}
bool ReadHelper::onClassWithMembersAndTypes(ClassWithMembersAndTypes &in, ObjectRef &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = libraries[in.libraryId];

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = ObjectRef(new UserClassTypeObject(in));
    }

    out = obj;

    // ReadMembers

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

QNRBF_END_NAMESPACE