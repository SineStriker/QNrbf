#include "ReadHelper.h"

#include <QDebug>

#include "Objects/DataObject.h"
#include "Objects/DeferredReferenceObject.h"
#include "Objects/MemberPrimitiveObject.h"
#include "Objects/OneOrMoreNullObject.h"
#include "Objects/SystemClassObject.h"
#include "Objects/SystemClassTypeObject.h"
#include "Objects/UserClassObject.h"
#include "Objects/UserClassTypeObject.h"

#include "Primitive/LengthPrefixedString.h"

#include "Records/BinaryMethodCall.h"
#include "Records/BinaryMethodReturn.h"
#include "Records/SerializationHeader.h"

QNRBF_BEGIN_NAMESPACE

static const int SuccessStatusMask = 1;

static QString posToStr(qint64 pos) {
    static const int print_base = 16;
    return "0x" + QString::number(pos, print_base).toUpper();
}

ReadHelper::ReadHelper(QDataStream *stream) : stream(stream) {
    hasHead = false;
    _status = Normal;
}

ReadHelper::~ReadHelper() {
}

ReadHelper::Status ReadHelper::status() const {
    return _status;
}

bool ReadHelper::read() {
    QSharedPointer<BinaryObject> obj;
    return readRecord(obj);
}

void ReadHelper::reset() {
    objects.clear();
    libraries.clear();

    hasHead = false;
    _status = Normal;
}

bool ReadHelper::readRecord(BinaryObjectRef &out) {
    QDataStream &in = *stream;
    QIODevice *dev = in.device();

    // Mark start position
    qint64 startPos = dev->pos();

    // Read record type enum
    quint8 recordType;
    in >> recordType;

    switch (recordType) {
        case (quint8) RecordTypeEnumeration::SerializedStreamHeader: {
            if (!hasHead) {
                SerializationHeader record;
                if (!record.read(in)) {
                    qDebug().noquote() << QString("QNrbfStream: read SerializedStreamHeader error "
                                                  "at %1, start from %2")
                                              .arg(posToStr(dev->pos()), posToStr(startPos));
                    _status = Failed;
                } else {
                    // res = WRAPP(record);
                }
                hasHead = true;
            } else {
                qDebug() << QString("QNrbfStream: multiple stream header, start from %1")
                                .arg(posToStr(startPos));
                _status = MultipleHead;
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ClassWithId: {
            ClassWithId record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read ClassWithId error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::SystemClassWithMembers: {
            SystemClassWithMembers record;
            if (!record.read(in)) {
                qDebug().noquote() << QString("QNrbfStream: read System error "
                                              "at %1, start from %2")
                                          .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ClassWithMembers: {
            ClassWithMembers record;
            if (!record.read(in)) {
                qDebug().noquote().noquote()
                    << QString("QNrbfStream: read User error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::SystemClassWithMembersAndTypes: {
            SystemClassWithMembersAndTypes record;
            if (!record.read(in)) {
                qDebug().noquote() << QString("QNrbfStream: read SystemWithTypes "
                                              "error at %1, start from %2")
                                          .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ClassWithMembersAndTypes: {
            ClassWithMembersAndTypes record;
            if (!record.read(in)) {
                qDebug().noquote() << QString("QNrbfStream: read UserWithTypes "
                                              "error at %1, start from %2")
                                          .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::BinaryObjectString: {
            BinaryObjectString record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read BinaryObjectString error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::BinaryArray: {
            BinaryArray record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read BinaryArray error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::MemberPrimitiveTyped: {
            MemberPrimitiveTyped record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read MemberPrimitiveTyped error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::MemberReference: {
            MemberReference record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read MemberReference error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ObjectNull: {
            break;
        }
        case (quint8) RecordTypeEnumeration::MessageEnd: {
            _status = ReachEnd;
            break;
        }
        case (quint8) RecordTypeEnumeration::BinaryLibrary: {
            BinaryLibrary record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read readBinaryLibrary error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ObjectNullMultiple256: {
            ObjectNullMultiple record;
            if (!record.read(in, true)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read ObjectNullMultiple256 error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ObjectNullMultiple: {
            ObjectNullMultiple record;
            if (!record.read(in, false)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read ObjectNullMultiple error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ArraySinglePrimitive: {
            ArraySinglePrimitive record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read ArraySinglePrimitive error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ArraySingleObject: {
            ArraySingleObject record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read ArraySingleObject error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::ArraySingleString: {
            ArraySingleString record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read ArraySingleString error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::MethodCall: {
            BinaryMethodCall record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read BinaryMethodCall error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        case (quint8) RecordTypeEnumeration::MethodReturn: {
            BinaryMethodReturn record;
            if (!record.read(in)) {
                qDebug().noquote()
                    << QString("QNrbfStream: read BinaryMethodReturn error at %1, start from %2")
                           .arg(posToStr(dev->pos()), posToStr(startPos));
                _status = Failed;
            } else {
                // res = WRAPP(record);
            }
            break;
        }
        default: {
            _status = UnsupportedRecord;
            break;
        }
    }

    return _status & SuccessStatusMask;
}

bool ReadHelper::readMembers(BinaryObject &acceptor, const QStringList &memberNames,
                             const MemberTypeInfo &memberTypeInfo) {
    return false;
}

bool ReadHelper::readUntypedMembers(BinaryObject &acceptor, const QString &className,
                                    const QStringList &memberNames) {
    return false;
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


QNRBF_END_NAMESPACE