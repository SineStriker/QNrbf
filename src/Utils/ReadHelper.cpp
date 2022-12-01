#include "ReadHelper.h"

#include <QDebug>

#include "Objects/DeferredReferenceObject.h"
#include "Objects/OneOrMoreNullObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/SystemClassObject.h"
#include "Objects/SystemClassTypeObject.h"
#include "Objects/UserClassObject.h"
#include "Objects/UserClassTypeObject.h"

#include "Primitive/Parser.h"

#include "Objects/ObjectListObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"
#include "Records/BinaryMethodCall.h"
#include "Records/BinaryMethodReturn.h"
#include "Records/SerializationHeader.h"

QNRBF_BEGIN_NAMESPACE

static const int SuccessStatusMask = 1;

static QString posToStr(qint64 pos) {
    static const int print_base = 16;
    return "0x" + QString::number(pos, print_base).toUpper();
}

template <class T>
static void resizeList(QList<T> &list, int size) {
    if (list.size() > size) {
        list.erase(list.begin() + size, list.end());
        return;
    }
    list.reserve(size);
    for (int i = list.size(); i < size; ++i) {
        list.append(T{});
    }
}

ReadHelper::ReadHelper(QDataStream *stream) : stream(stream) {
    _status = Normal;
}

ReadHelper::~ReadHelper() {
}

ReadHelper::Status ReadHelper::status() const {
    return _status;
}

bool ReadHelper::read() {
    return readRecord();
}

void ReadHelper::reset() {
    objectsById.clear();
    classesById.clear();
    libraries.clear();
    deferredItems.clear();

    header.clear();
    _status = Normal;
}

bool ReadHelper::finish(ObjectRef *out) {
    resolveDeferredItems();
    if (out) {
        *out = findReference(header->rootId);
    }
    return true;
}

bool ReadHelper::readRecord(ObjectRef *out) {
    QDataStream &in = *stream;
    QIODevice *dev = in.device();

    // Mark start position
    qint64 startPos = dev->pos();

    // Read record type enum
    quint8 recordType;
    in >> recordType;

    QSharedPointer<AbstractObject> obj;
    switch (recordType) {
        case (quint8) RecordTypeEnumeration::SerializedStreamHeader: {
            if (header.isNull()) {
                header = QSharedPointer<SerializationHeader>::create();
                if (!header->read(in)) {
                    header.clear();
                    _status = Failed;
                }
            } else {
                qDebug() << QString("QNrbfStream: multiple stream header, start from %1")
                                .arg(posToStr(startPos));
                _status = MultipleHead;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ClassWithId: {
            ClassWithId record;
            if (!record.read(in) || !onClassWithId(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::SystemClassWithMembers: {
            SystemClassWithMembers record;
            if (!record.read(in) || !onSystemClassWithMembers(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ClassWithMembers: {
            ClassWithMembers record;
            if (!record.read(in) || !onClassWithMembers(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::SystemClassWithMembersAndTypes: {
            SystemClassWithMembersAndTypes record;
            if (!record.read(in) || !onSystemClassWithMembersAndTypes(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ClassWithMembersAndTypes: {
            ClassWithMembersAndTypes record;
            if (!record.read(in) || !onClassWithMembersAndTypes(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::BinaryArray: {
            BinaryArray record;
            if (!record.read(in) || !onBinaryArray(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::BinaryLibrary: {
            BinaryLibrary record;
            if (!record.read(in) || !onBinaryLibrary(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ArraySinglePrimitive: {
            ArraySinglePrimitive record;
            if (!record.read(in) || !onArraySinglePrimitive(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ArraySingleObject: {
            ArraySingleObject record;
            if (!record.read(in) || !onArraySingleObject(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ArraySingleString: {
            ArraySingleString record;
            if (!record.read(in) || !onArraySingleString(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::MessageEnd: {
            _status = ReachEnd;
            break;
        }
        case (quint8) RecordTypeEnumeration::MethodCall: {
            BinaryMethodCall record;
            if (!record.read(in)) {
                _status = Failed;
            } else {
                // Not implemented
                // ...

                qDebug().noquote() << QString("QNrbfStream: BinaryMethodCall not implemented...");
                _status = UnsupportedRecord;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::MethodReturn: {
            BinaryMethodReturn record;
            if (!record.read(in)) {
                _status = Failed;
            } else {
                // Not implemented
                // ...

                qDebug().noquote() << QString("QNrbfStream: BinaryMethodReturn not implemented...");
                _status = UnsupportedRecord;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::BinaryObjectString: {
            BinaryObjectString record;
            if (!record.read(in) || !onBinaryObjectString(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::MemberPrimitiveTyped: {
            MemberPrimitiveTyped record;
            if (!record.read(in) || !onMemberPrimitiveTyped(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::MemberReference: {
            MemberReference record;
            if (!record.read(in) || !onMemberReference(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ObjectNull: {
            if (!onObjectNull(obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ObjectNullMultiple256: {
            ObjectNullMultiple record;
            if (!record.read(in, true) || !onObjectNullMultiple(record, obj)) {
                _status = Failed;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ObjectNullMultiple: {
            ObjectNullMultiple record;
            if (!record.read(in, false) || !onObjectNullMultiple(record, obj)) {
                _status = Failed;
            }
            break;
        }
        default: {
            _status = UnsupportedRecord;
            break;
        }
    }

    if (_status == Failed) {
        qDebug().noquote().noquote()
            << QString("QNrbfStream: read %1 error at %2, start from %3")
                   .arg(Parser::strRecordTypeEnum((RecordTypeEnumeration) recordType),
                        posToStr(dev->pos()), posToStr(startPos));
    }

    bool success = _status & SuccessStatusMask;
    if (success && out) {
        *out = obj;
    }
    return success;
}

bool ReadHelper::readMembers(const MappingRef &acceptor, const QStringList &memberNames,
                             const MemberTypeInfo &memberTypeInfo) {
    QDataStream &in = *stream;
    for (int i = 0; i < memberNames.size(); i++) {
        if (memberTypeInfo.binaryTypeEnums[i] == BinaryTypeEnumeration::Primitive) {
            PrimitiveValue value;
            if (!value.read(in, memberTypeInfo.additionalInfos[i].toPrimitiveTypeEnum())) {
                return false;
            }
            acceptor->members.insert(memberNames[i], ObjectRef(new PrimitiveObject(value)));
        } else {
            ObjectRef memberClass;
            if (!readRecord(&memberClass)) {
                return false;
            }
            if (memberClass->type() == AbstractObject::DeferredReference) {

                // Action: insert a key-value-like member to object
                struct Action : DeferredItem::Action {
                    QSharedPointer<MappingObject> parent;
                    QString member;
                    Action(const QSharedPointer<MappingObject> &parent, const QString &member)
                        : parent(parent), member(member) {
                    }
                    void Invoke(const ObjectRef &obj) override {
                        parent->members.insert(member, obj);
                    }
                };

                deferredItems.append(
                    DeferredItem(new Action(acceptor, memberNames[i]),
                                 dynamic_cast<DeferredReferenceObject *>(memberClass.data())->id));
            } else {
                acceptor->members.insert(memberNames[i], memberClass);
            }
        }
    }
    return true;
}

bool ReadHelper::readUntypedMembers(const MappingRef &acceptor, const QString &className,
                                    const QStringList &memberNames) {
    QDataStream &in = *stream;

    qDebug() << "read untyped members" << memberNames;

#define ADD_MEMBER(KEY, TYPE)                                                                      \
    {                                                                                              \
        q##TYPE val_##TYPE;                                                                        \
        in >> val_##TYPE;                                                                          \
        if (in.status() != QDataStream::Ok)                                                        \
            return false;                                                                          \
        acceptor->members.insert(KEY, ObjectRef(new PrimitiveObject(val_##TYPE)));                 \
    }

    if (className == "System.Guid" && memberNames.size() == 11) {
        ADD_MEMBER("_a", int32);
        ADD_MEMBER("_b", int16);
        ADD_MEMBER("_c", int16);
        ADD_MEMBER("_d", int8);
        ADD_MEMBER("_e", int8);
        ADD_MEMBER("_f", int8);
        ADD_MEMBER("_g", int8);
        ADD_MEMBER("_h", int8);
        ADD_MEMBER("_i", int8);
        ADD_MEMBER("_j", int8);
        ADD_MEMBER("_k", int8);
    } else if (memberNames.size() == 1) {
        if (memberNames[0] == "value__") {
            // Likely an enum but we don't know the size. Take a chance at the default int
            ADD_MEMBER(memberNames.front(), int32);
            return true;
        }
    }

    qDebug().noquote() << QString("QNrbfStream: Unsupported untyped member: %1").arg(className);
    return false;

#undef ADD_MEMBER
}

bool ReadHelper::onSystemClassWithMembers(SystemClassWithMembers &in, ObjectRef &out) {
    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = in.classInfo.name;

    // Save the defined class information
    if (in.classInfo.objectId != 0) {
        classesById[in.classInfo.objectId] = ClassRef(new SystemClassObject(in, obj));
    }

    // Read members
    if (!readUntypedMembers(obj, in.classInfo.name, in.classInfo.memberNames)) {
        return false;
    }

    out = obj;
    return true;
}

bool ReadHelper::onClassWithMembers(ClassWithMembers &in, ObjectRef &out) {
    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = libraries[in.libraryId];

    // Save the defined class information
    if (in.classInfo.objectId != 0) {
        classesById[in.classInfo.objectId] = ClassRef(new UserClassObject(in, obj));
    }

    // Read members
    if (!readUntypedMembers(obj, in.classInfo.name, in.classInfo.memberNames)) {
        return false;
    }

    out = obj;
    return true;
}
bool ReadHelper::onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in,
                                                  ObjectRef &out) {
    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = in.classInfo.name;

    // Save the defined class information
    if (in.classInfo.objectId != 0) {
        classesById[in.classInfo.objectId] = ClassRef(new SystemClassTypeObject(in, obj));
    }

    // Read members
    if (!readMembers(obj, in.classInfo.memberNames, in.memberTypeInfo)) {
        return false;
    }

    out = obj;
    return true;
}
bool ReadHelper::onClassWithMembersAndTypes(ClassWithMembersAndTypes &in, ObjectRef &out) {
    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = libraries[in.libraryId];

    // Save the defined class information
    if (in.classInfo.objectId != 0) {
        classesById[in.classInfo.objectId] = ClassRef(new UserClassTypeObject(in, obj));
    }

    // Read members
    if (!readMembers(obj, in.classInfo.memberNames, in.memberTypeInfo)) {
        return false;
    }

    out = obj;
    return true;
}

bool ReadHelper::onClassWithId(ClassWithId &in, ObjectRef &out) {
    // This class has been defined, and we use the saved one
    auto classRef = dynamic_cast<ClassMemberObject *>(classesById[in.metadataId].data());
    if (!classRef) {
        return false;
    }
    auto objRef = classRef->value.data();

    // Copy information
    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = objRef->typeName;
    obj->assemblyName = objRef->assemblyName;

    // Save object reference
    if (in.objectId != 0) {
        objectsById[in.objectId] = obj;
    }

    // Read members
    if (classRef->classType() & ClassMemberObject::WithTypes) {
        if (!readMembers(obj, classRef->classInfo.memberNames, classRef->memberTypeInfo)) {
            return false;
        }
    } else {
        if (!readUntypedMembers(obj, obj->typeName, classRef->classInfo.memberNames)) {
            return false;
        }
    }

    out = obj;
    return true;
}

bool ReadHelper::onBinaryObjectString(BinaryObjectString &in, ObjectRef &out) {
    auto obj = ObjectRef(new StringObject(in.value));
    if (in.objectId != 0) {
        objectsById[in.objectId] = obj;
    }
    out = obj;
    return true;
}

bool ReadHelper::onBinaryArray(BinaryArray &in, ObjectRef &out) {
    int production = 1;
    for (int num : qAsConst(in.lengths)) {
        production *= num;
    }

    QSharedPointer<AbstractObject> obj;
    switch (in.additionInfo.type()) {
        case RemotingTypeInfo::PrimitiveType: {
            PrimitiveValueArray arr;
            if (!arr.read(*stream, production, in.additionInfo.toPrimitiveTypeEnum())) {
                return false;
            }
            auto listObj = new PrimitiveListObject(arr);
            listObj->lengths = in.lengths;
            listObj->lowerBounds = in.lowerBounds;
            obj = ObjectRef(listObj);
            break;
        }
        case RemotingTypeInfo::String: {
            QStringList arr;
            resizeList(arr, production);
            if (!readStrings(arr)) {
                return false;
            }
            auto listObj = new StringListObject(arr);
            listObj->lengths = in.lengths;
            listObj->lowerBounds = in.lowerBounds;
            obj = ObjectRef(listObj);
            break;
        }
        default: {
            QSharedPointer<ObjectListObject> listObj(new ObjectListObject());
            resizeList(listObj->values, production);
            if (!readObjects(listObj->values, listObj)) {
                return false;
            }
            listObj->lengths = in.lengths;
            listObj->lowerBounds = in.lowerBounds;
            obj = listObj;
            break;
        }
    }

    if (in.objectId != 0) {
        objectsById[in.objectId] = obj;
    }
    out = obj;

    return true;
}

bool ReadHelper::onMemberPrimitiveTyped(MemberPrimitiveTyped &in, ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new PrimitiveObject(in.value));
    return true;
}

bool ReadHelper::onMemberReference(MemberReference &in, ObjectRef &out) {
    auto searchObj = findReference(in.idRef);
    if (searchObj) {
        out = searchObj;
    } else {
        // Since there's no restriction of the order of definition and reference,
        // The target definition may appear later, so we need to defer the dereference work

        out = ObjectRef(new DeferredReferenceObject(in.idRef));
    }
    return true;
}

bool ReadHelper::onBinaryLibrary(BinaryLibrary &in, ObjectRef &out) {
    Q_UNUSED(out);
    libraries[in.libraryId] = in.libraryName;
    return true;
}

bool ReadHelper::onObjectNull(ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new OneOrMoreNullObject());
    return true;
}

bool ReadHelper::onObjectNullMultiple(ObjectNullMultiple &in, ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new OneOrMoreNullObject(in.nullCount));
    return true;
}

bool ReadHelper::onArraySinglePrimitive(ArraySinglePrimitive &in, ObjectRef &out) {
    PrimitiveValueArray arr;
    if (!arr.read(*stream, in.arrayInfo.length, in.primitiveTypeEnum)) {
        return false;
    }
    auto obj = ObjectRef(new PrimitiveListObject(arr));
    if (in.arrayInfo.objectId != 0) {
        objectsById[in.arrayInfo.objectId] = obj;
    }
    out = obj;

    return true;
}

bool ReadHelper::onArraySingleObject(ArraySingleObject &in, ObjectRef &out) {
    // Allocate first because the read function needs it
    auto obj = QSharedPointer<ObjectListObject>::create();
    resizeList(obj->values, in.arrayInfo.length);
    if (!readObjects(obj->values, obj)) {
        return false;
    }

    if (in.arrayInfo.objectId != 0) {
        objectsById[in.arrayInfo.objectId] = obj;
    }
    out = obj;

    return true;
}

bool ReadHelper::onArraySingleString(ArraySingleString &in, ObjectRef &out) {
    QStringList arr;
    resizeList(arr, in.arrayInfo.length);

    if (!readStrings(arr)) {
        return false;
    }

    auto obj = ObjectRef(new PrimitiveListObject(arr));
    if (in.arrayInfo.objectId != 0) {
        objectsById[in.arrayInfo.objectId] = obj;
    }
    out = obj;

    return true;
}

bool ReadHelper::readStrings(QStringList &arr) {
    for (int i = 0; i < arr.size(); ++i) {
        // Read next object
        QSharedPointer<AbstractObject> value;
        if (!readRecord(&value)) {
            return false;
        }
        switch (value->type()) {
            case AbstractObject::Null: {
                i += dynamic_cast<OneOrMoreNullObject *>(value.data())->nullCount - 1;
                break;
            }
            case AbstractObject::String: {
                arr[i] = dynamic_cast<StringObject *>(value.data())->value;
                break;
            }
            default: {
                // Ignore this one
                i--;
                break;
            }
        }
    }
    return true;
}

bool ReadHelper::readObjects(QList<ObjectRef> &arr, const QSharedPointer<ObjectListObject> &parent) {
    for (int i = 0; i < arr.size(); i++) {
        // Read next object
        QSharedPointer<AbstractObject> value;
        if (!readRecord(&value)) {
            return false;
        }

        // Save according to type
        switch (value->type()) {
            case AbstractObject::DeferredReference: {

                // Action: set the element of object by the given index
                struct Action : DeferredItem::Action {
                    int index;
                    QSharedPointer<ObjectListObject> parent;
                    Action(int index, const QSharedPointer<ObjectListObject> &parent)
                        : index(index), parent(parent) {
                    }
                    void Invoke(const ObjectRef &obj) override {
                        parent->values[index] = obj;
                    }
                };

                deferredItems.append(
                    DeferredItem(new Action(i, parent),
                                 dynamic_cast<DeferredReferenceObject *>(value.data())->id));
                break;
            }
            case AbstractObject::Null: {
                i += dynamic_cast<OneOrMoreNullObject *>(value.data())->nullCount - 1;
                break;
            }
            case AbstractObject::Primitive:
            case AbstractObject::String: {
                arr[i] = value;
                break;
            }
            default: {
                // Ignore
                i--;
                break;
            }
        }
    }
    return true;
}

void ReadHelper::resolveDeferredItems() {
    for (auto &item : deferredItems) {
        auto refItem = findReference(item.id);
        item.deferredAction->Invoke(refItem);
    }
}

ObjectRef ReadHelper::findReference(qint32 id) {
    // Search class
    {
        auto it = classesById.find(id);
        if (it != classesById.end()) {
            return dynamic_cast<ClassMemberObject *>(it.value().data())->value;
        }
    }
    // Search object
    {
        auto it = objectsById.find(id);
        if (it != objectsById.end()) {
            return it.value();
        }
    }
    return nullptr;
}

QNRBF_END_NAMESPACE
