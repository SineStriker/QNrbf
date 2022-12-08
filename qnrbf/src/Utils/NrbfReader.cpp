#include "NrbfReader.h"

#include <QDebug>

#include "Objects/DeferredReferenceObject.h"
#include "Objects/ObjectListObject.h"
#include "Objects/OneOrMoreNullObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"
#include "Objects/SystemClassObject.h"
#include "Objects/SystemClassTypeObject.h"
#include "Objects/UserClassObject.h"
#include "Objects/UserClassTypeObject.h"

#include "Records/BinaryMethodCall.h"
#include "Records/BinaryMethodReturn.h"
#include "Records/SerializationHeader.h"

#include "NrbfHelper.h"
#include "NrbfParser.h"

QNRBF_BEGIN_NAMESPACE

static const int SuccessStatusMask = 1;

NrbfReader::NrbfReader(QDataStream *stream) : stream(stream) {
    _status = Normal;
}

NrbfReader::~NrbfReader() {
}

NrbfReader::Status NrbfReader::status() const {
    return _status;
}

NrbfRegistry NrbfReader::read() {
    if (_status != Normal) {
        reset();
    }

    bool over = false;
    bool failed = false;

    while (!over && !failed) {
        readRecord();

        switch (_status) {
            case ReachEnd: {
                over = true;
                break;
            }

            case Normal:
                break;

            default: {
                failed = true;
                break;
            }
        }
    }

    return reg;
}

void NrbfReader::reset() {
    reg.reset();

    _status = Normal;
}

bool NrbfReader::readRecord(ObjectRef *out) {
    QDataStream &in = *stream;
    QIODevice *dev = in.device();

    // Mark start position
    qint64 startPos = dev->pos();

    // Read record type enum
    quint8 recordType;
    in >> recordType;

    QSharedPointer<AbstractObject> obj;

    // qDebug().noquote() << posToStr(startPos)
    //                    << Parser::strRecordTypeEnum(RecordTypeEnumeration(recordType));

    switch (recordType) {
        case (quint8) RecordTypeEnumeration::SerializedStreamHeader: {
            if (reg.header.isNull()) {
                SerializationHeader header;
                if (header.read(in)) {
                    reg.header = QSharedPointer<SerializationHeader>::create(std::move(header));
                } else {
                    _status = Failed;
                }
            } else {
                qDebug().noquote() << QString("NrbfReader: Multiple stream header, start from %1")
                                          .arg(Helper::num2hex(startPos));
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

                qDebug().noquote()
                    << QString("NrbfReader: BinaryMethodCall not implemented, start from %2")
                           .arg(QString::number(recordType), Helper::num2hex(startPos));
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

                qDebug().noquote()
                    << QString("NrbfReader: BinaryMethodReturn not implemented, start from %2")
                           .arg(QString::number(recordType), Helper::num2hex(startPos));
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
            // May be unreachable
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
            qDebug().noquote() << QString("NrbfReader: Unknown record type %1, start from %2")
                                      .arg(QString::number(recordType), Helper::num2hex(startPos));
            _status = UnsupportedRecord;
            break;
        }
    }

    if (_status == Failed) {
        qDebug().noquote() << QString("NrbfReader: Read %1 error at %2, start from %3")
                                  .arg(
                                      Parser::strRecordTypeEnum((RecordTypeEnumeration) recordType),
                                      Helper::num2hex(dev->pos()), Helper::num2hex(startPos));
    }

    bool success = _status & SuccessStatusMask;
    if (success && out) {
        *out = obj;
    }
    return success;
}

bool NrbfReader::readMembers(const MappingRef &acceptor, const QStringList &memberNames,
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
            acceptor->members.insert(memberNames[i], memberClass);
        }
    }
    return true;
}

bool NrbfReader::readUntypedMembers(const MappingRef &acceptor, const QString &className,
                                    const QStringList &memberNames) {
    QDataStream &in = *stream;

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

    qDebug().noquote() << QString("NrbfReader: Unsupported untyped member: %1").arg(className);
    return false;

#undef ADD_MEMBER
}

bool NrbfReader::onSystemClassWithMembers(SystemClassWithMembers &in, ObjectRef &out) {
    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = in.classInfo.name;
    obj->parentInfo = "SystemClassWithMembers";

    obj->id = in.classInfo.objectId;
    obj->classId = obj->id;

    // Save the defined class information
    if (in.classInfo.objectId != 0) {
        reg.classesById[in.classInfo.objectId] = ClassRef(new SystemClassObject(in, obj));
    }

    // Read members
    if (!readUntypedMembers(obj, in.classInfo.name, in.classInfo.memberNames)) {
        return false;
    }

    out = obj;
    return true;
}

bool NrbfReader::onClassWithMembers(ClassWithMembers &in, ObjectRef &out) {
    // Find library name
    QString library;
    {
        auto it = reg.libraries.find(in.libraryId);
        if (it == reg.libraries.end()) {
            qDebug().noquote() << QString("NrbfReader: Undefined class library id %1")
                                      .arg(QString::number(in.libraryId));
            return false;
        }
        library = it.value();
    }

    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = library;
    obj->parentInfo = "ClassWithMembers";

    obj->id = in.classInfo.objectId;
    obj->classId = obj->id;

    // Save the defined class information
    if (in.classInfo.objectId != 0) {
        reg.classesById[in.classInfo.objectId] = ClassRef(new UserClassObject(in, obj));
    }

    // Read members
    if (!readUntypedMembers(obj, in.classInfo.name, in.classInfo.memberNames)) {
        return false;
    }

    out = obj;
    return true;
}
bool NrbfReader::onSystemClassWithMembersAndTypes(SystemClassWithMembersAndTypes &in,
                                                  ObjectRef &out) {
    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = in.classInfo.name;
    obj->parentInfo = "SystemClassWithMembersAndTypes";

    obj->id = in.classInfo.objectId;
    obj->classId = obj->id;

    // Save the defined class information
    if (in.classInfo.objectId != 0) {
        reg.classesById[in.classInfo.objectId] = ClassRef(new SystemClassTypeObject(in, obj));
    }

    // Read members
    if (!readMembers(obj, in.classInfo.memberNames, in.memberTypeInfo)) {
        qDebug().noquote() << "NrbfReader: Read class members failed";
        return false;
    }

    out = obj;
    return true;
}
bool NrbfReader::onClassWithMembersAndTypes(ClassWithMembersAndTypes &in, ObjectRef &out) {
    // Find library name
    QString library;
    {
        auto it = reg.libraries.find(in.libraryId);
        if (it == reg.libraries.end()) {
            qDebug().noquote() << QString("NrbfReader: Undefined class library id %1")
                                      .arg(QString::number(in.libraryId));
            return false;
        }
        library = it.value();
    }

    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = in.classInfo.name;
    obj->assemblyName = library;
    obj->parentInfo = "ClassWithMembersAndTypes";

    obj->id = in.classInfo.objectId;
    obj->classId = obj->id;

    // Save the defined class information
    if (in.classInfo.objectId != 0) {
        reg.classesById[in.classInfo.objectId] = ClassRef(new UserClassTypeObject(in, obj));
    }

    // Read members
    if (!readMembers(obj, in.classInfo.memberNames, in.memberTypeInfo)) {
        qDebug().noquote() << "NrbfReader: Read class members failed";
        return false;
    }

    out = obj;
    return true;
}

bool NrbfReader::onClassWithId(ClassWithId &in, ObjectRef &out) {
    // Find saved class
    auto it = reg.classesById.find(in.metadataId);
    if (it == reg.classesById.end()) {
        qDebug().noquote() << QString("NrbfReader: Undefined class reference id %1")
                                  .arg(QString::number(in.metadataId));
        return false;
    }

    // This class has been defined, and we use the saved one
    auto classRef = it.value();
    auto objRef = classRef->value.data();

    // Copy information
    auto obj = QSharedPointer<MappingObject>::create();
    obj->typeName = objRef->typeName;
    obj->assemblyName = objRef->assemblyName;
    obj->parentInfo = QString("ClassWithId %1").arg(QString::number(in.metadataId));

    obj->id = in.objectId;
    obj->classId = in.metadataId;

    // Save object reference
    if (in.objectId != 0) {
        reg.objectsById[in.objectId] = obj;
    }

    // Read members
    if (classRef->classType() & ClassMemberObject::WithTypes) {
        if (!readMembers(obj, classRef->classInfo.memberNames, classRef->memberTypeInfo)) {
            qDebug().noquote() << "NrbfReader: Read class members failed";
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

bool NrbfReader::onBinaryObjectString(BinaryObjectString &in, ObjectRef &out) {
    auto obj = ObjectRef(new StringObject(in.value));
    if (in.objectId != 0) {
        obj->id = in.objectId;
        reg.objectsById[in.objectId] = obj;
    }
    out = obj;
    return true;
}

bool NrbfReader::onBinaryArray(BinaryArray &in, ObjectRef &out) {
    int production = 1;
    for (int num : qAsConst(in.lengths)) {
        production *= num;
    }

    QSharedPointer<AbstractObject> obj;
    switch (in.additionInfo.type()) {
        case RemotingTypeInfo::PrimitiveType: {
            PrimitiveValueArray arr;
            if (!arr.read(*stream, production, in.additionInfo.toPrimitiveTypeEnum())) {
                qDebug().noquote() << QString("NrbfReader: Read primitive array failed");
                return false;
            }
            auto listObj = new PrimitiveListObject(arr);
            listObj->lengths = in.lengths;
            listObj->lowerBounds = in.lowerBounds;
            listObj->hasShapeInfo = true;
            listObj->shapeInfo = in;
            obj = ObjectRef(listObj);
            break;
        }
        case RemotingTypeInfo::String: {
            QStringList arr;
            Helper::resizeList(arr, production);
            if (!readStrings(arr)) {
                qDebug().noquote() << QString("NrbfReader: Read string array failed");
                return false;
            }
            auto listObj = new StringListObject(arr);
            listObj->lengths = in.lengths;
            listObj->lowerBounds = in.lowerBounds;
            listObj->hasShapeInfo = true;
            listObj->shapeInfo = in;
            obj = ObjectRef(listObj);
            break;
        }
        default: {
            QSharedPointer<ObjectListObject> listObj(new ObjectListObject());
            Helper::resizeList(listObj->values, production);
            if (!readObjects(listObj->values)) {
                qDebug().noquote() << QString("NrbfReader: Read object array failed");
                return false;
            }
            listObj->lengths = in.lengths;
            listObj->lowerBounds = in.lowerBounds;
            listObj->hasShapeInfo = true;
            listObj->shapeInfo = in;
            obj = listObj;
            break;
        }
    }

    if (in.objectId != 0) {
        obj->id = in.objectId;
        reg.objectsById[in.objectId] = obj;
    }
    out = obj;

    return true;
}

bool NrbfReader::onMemberPrimitiveTyped(MemberPrimitiveTyped &in, ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new PrimitiveObject(in.value));
    return true;
}

bool NrbfReader::onMemberReference(MemberReference &in, ObjectRef &out) {
    auto searchObj = reg.findReference(in.idRef);
    if (searchObj) {
        out = searchObj;
    } else {
        // Since there's no restriction of the order of definition and reference,
        // The target definition may appear later, so we need to defer the dereference work

        out = ObjectRef(new DeferredReferenceObject(in.idRef));
    }
    return true;
}

bool NrbfReader::onBinaryLibrary(BinaryLibrary &in, ObjectRef &out) {
    Q_UNUSED(out);
    reg.libraries[in.libraryId] = in.libraryName;
    return true;
}

bool NrbfReader::onObjectNull(ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new OneOrMoreNullObject());
    return true;
}

bool NrbfReader::onObjectNullMultiple(ObjectNullMultiple &in, ObjectRef &out) {
    Q_UNUSED(this);
    out = ObjectRef(new OneOrMoreNullObject(in.nullCount));
    return true;
}

bool NrbfReader::onArraySinglePrimitive(ArraySinglePrimitive &in, ObjectRef &out) {
    PrimitiveValueArray arr;
    if (!arr.read(*stream, in.arrayInfo.length, in.primitiveTypeEnum)) {
        qDebug().noquote() << QString("NrbfReader: Read primitive array failed");
        return false;
    }

    auto obj = QSharedPointer<PrimitiveListObject>::create(arr);
    if (in.arrayInfo.objectId != 0) {
        obj->id = in.arrayInfo.objectId;
        reg.objectsById[in.arrayInfo.objectId] = obj;
    }

    out = obj;

    return true;
}

bool NrbfReader::onArraySingleObject(ArraySingleObject &in, ObjectRef &out) {
    // Allocate first because the read function needs it
    auto obj = QSharedPointer<ObjectListObject>::create();
    Helper::resizeList(obj->values, in.arrayInfo.length);
    if (!readObjects(obj->values)) {
        qDebug().noquote() << QString("NrbfReader: Read object array failed");
        return false;
    }

    if (in.arrayInfo.objectId != 0) {
        obj->id = in.arrayInfo.objectId;
        reg.objectsById[in.arrayInfo.objectId] = obj;
    }

    out = obj;

    return true;
}

bool NrbfReader::onArraySingleString(ArraySingleString &in, ObjectRef &out) {
    QStringList arr;
    Helper::resizeList(arr, in.arrayInfo.length);
    if (!readStrings(arr)) {
        qDebug().noquote() << QString("NrbfReader: Read string array failed");
        return false;
    }

    auto obj = QSharedPointer<StringListObject>::create(arr);
    if (in.arrayInfo.objectId != 0) {
        obj->id = in.arrayInfo.objectId;
        reg.objectsById[in.arrayInfo.objectId] = obj;
    }

    out = obj;

    return true;
}

bool NrbfReader::readStrings(QStringList &arr) {
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

bool NrbfReader::readObjects(QList<ObjectRef> &arr) {
    for (int i = 0; i < arr.size(); i++) {
        // Read next object
        QSharedPointer<AbstractObject> value;
        if (!readRecord(&value)) {
            return false;
        }

        // Save according to type
        switch (value->type()) {
            case AbstractObject::Null: {
                i += dynamic_cast<OneOrMoreNullObject *>(value.data())->nullCount - 1;
                break;
            }
            case AbstractObject::DeferredReference:
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

QNRBF_END_NAMESPACE
