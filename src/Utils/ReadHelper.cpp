#include "ReadHelper.h"

#include "Common/DataRecord.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::ReadHelper::ReadHelper(QDataStream *stream) : stream(stream) {
    Q_ASSERT(stream);
}

ReadHelper::~ReadHelper() {
}

bool ReadHelper::onClassWithId(ClassWithId &in, QSharedPointer<AbstractObject> &out) {
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

bool ReadHelper::onSystemClassWithMembers(SystemClassWithMembers &in,
                                          QSharedPointer<AbstractObject> &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] =
            QSharedPointer<AbstractObject>(new SystemClassWithMembers(in));
    }

    out = in.value;

    // ReadUntypedMembers

    return true;
}
bool ReadHelper::onClassWithMembers(ClassWithMembers &in, QSharedPointer<AbstractObject> &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = libraries[in.libraryId];

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] = QSharedPointer<AbstractObject>(new ClassWithMembers(in));
    }

    out = in.value;

    // ReadUntypedMembers

    return true;
}
bool ReadHelper::onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in,
                                                  QSharedPointer<AbstractObject> &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] =
            QSharedPointer<AbstractObject>(new SystemClassWithMembersAndTypes(in));
    }

    out = in.value;

    // ReadMembers

    return true;
}
bool ReadHelper::onClassWithMembersAndTypes(ClassWithMembersAndTypes &in,
                                            QSharedPointer<AbstractObject> &out) {
    auto obj = QSharedPointer<BinaryObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = libraries[in.libraryId];

    if (in.classInfo.objectId != 0) {
        objects[in.classInfo.objectId] =
            QSharedPointer<AbstractObject>(new ClassWithMembersAndTypes(in));
    }

    out = in.value;

    // ReadMembers

    return true;
}

bool ReadHelper::onBinaryObjectString(BinaryObjectString &in, QSharedPointer<AbstractObject> &out) {

    if (in.objectId != 0) {
        objects[in.objectId] = QSharedPointer<AbstractObject>(new DataRecord(in.value));
    }

    return true;
}

bool ReadHelper::onBinaryArray(BinaryArray &in, QSharedPointer<AbstractObject> &out) {
    return true;
}

QNRBF_END_NAMESPACE