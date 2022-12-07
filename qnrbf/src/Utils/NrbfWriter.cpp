#include "NrbfWriter.h"

#include "Enums/RecordTypeEnumeration.h"
#include "Objects/DeferredReferenceObject.h"
#include "Objects/ObjectListObject.h"
#include "Objects/OneOrMoreNullObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"
#include "Primitive/Parser.h"

QNRBF_BEGIN_NAMESPACE

static void errorWriteFailed() {
    qDebug() << "NrbfWriter: Write failed";
}

NrbfWriter::NrbfWriter(QDataStream *stream) : stream(stream) {
}

NrbfWriter::NrbfWriter(QDataStream *stream, const NrbfRegistry &reg) : reg(reg), stream(stream) {
}

NrbfWriter::~NrbfWriter() {
}

void NrbfWriter::setRegistry(const NrbfRegistry &reg) {
    this->reg = reg;
}

bool NrbfWriter::write() {
    if (reg.header.isNull()) {
        return false;
    }

    QDataStream &out = *stream;

    writtenObjects.clear();

    // Write header
    if (!reg.header->write(out)) {
        qDebug() << "NrbfWriter: Failed to write header";
        return false;
    }

    // Write libraries
    for (auto it = reg.libraries.begin(); it != reg.libraries.end(); ++it) {
        BinaryLibrary record;
        record.libraryId = it.key();
        record.libraryName = it.value();

        if (!record.write(out)) {
            qDebug() << "NrbfWriter: Failed to write libraries";
            return false;
        }
    }

    // Write objects
    auto it1 = reg.classesById.begin();
    auto it2 = reg.objectsById.begin();
    while (it1 != reg.classesById.end() || it2 != reg.objectsById.end()) {
        qint32 key;
        ObjectRef objRef;
        if (it2 == reg.objectsById.end() || it1.key() < it2.key()) {
            key = it1.key();
            objRef = it1.value();
            it1++;
        } else {
            key = it2.key();
            objRef = it2.value();
            it2++;
        }
        if (writtenObjects.contains(key)) {
            continue;
        }
        if (!writeObject(objRef)) {
            return false;
        }
    }

    return true;
}

bool NrbfWriter::writeObject(const ObjectRef &objRef) {
    writtenObjects.insert(objRef->id);

    QDataStream &out = *stream;
    switch (objRef->type()) {
        case AbstractObject::ClassMember: {
            auto classRef = objRef.dynamicCast<ClassMemberObject>();
            switch (classRef->classType()) {
                case ClassMemberObject::UserWithTypes: {
                    // Write record type
                    out << quint8(RecordTypeEnumeration::ClassWithMembersAndTypes);
                    if (out.status() != QDataStream::Ok) {
                        errorWriteFailed();
                        return false;
                    }

                    // Write record
                    ClassWithMembersAndTypes record;
                    record.classInfo = classRef->classInfo;
                    record.memberTypeInfo = classRef->memberTypeInfo;
                    record.libraryId = classRef->libraryId;

                    if (!record.write(out)) {
                        errorWriteFailed();
                        return false;
                    }

                    // Write members
                    if (!writeMembers(classRef->value, classRef->classInfo.memberNames,
                                      classRef->memberTypeInfo)) {
                        return false;
                    }
                    break;
                }
                case ClassMemberObject::SystemWithTypes: {
                    // Write record type
                    out << quint8(RecordTypeEnumeration::SystemClassWithMembersAndTypes);
                    if (out.status() != QDataStream::Ok) {
                        errorWriteFailed();
                        return false;
                    }

                    // Write record
                    SystemClassWithMembersAndTypes record;
                    record.classInfo = classRef->classInfo;
                    record.memberTypeInfo = classRef->memberTypeInfo;

                    if (!record.write(out)) {
                        errorWriteFailed();
                        return false;
                    }

                    // Write members
                    if (!writeMembers(classRef->value, classRef->classInfo.memberNames,
                                      classRef->memberTypeInfo)) {
                        return false;
                    }
                    break;
                }
                default: {
                    qDebug() << "NrbfWritter: Unsupported class record without types";
                    return false;

                    break;
                }
            }
            break;
        }
        case AbstractObject::Mapping: {
            auto mapping = objRef.dynamicCast<MappingObject>();
            ClassRef classRef;
            // Find class definition
            {
                auto it = reg.classesById.find(mapping->classId);
                if (it == reg.classesById.end()) {
                    qDebug().noquote() << QString("NrbfWritter: Undefined class reference id %1")
                                              .arg(QString::number(mapping->classId));
                    return false;
                }
                classRef = it.value();
            }

            // Write record type
            out << quint8(RecordTypeEnumeration::ClassWithId);
            if (out.status() != QDataStream::Ok) {
                errorWriteFailed();
                return false;
            }

            // Write record
            ClassWithId record;
            record.objectId = mapping->id;
            record.metadataId = mapping->classId;

            if (!record.write(out)) {
                errorWriteFailed();
                return false;
            }

            // Write members
            if (!writeMembers(mapping, classRef->classInfo.memberNames, classRef->memberTypeInfo)) {
                return false;
            }
            break;
        }
        case AbstractObject::DeferredReference: {
            auto obj = objRef.dynamicCast<DeferredReferenceObject>();
            // Write record type
            out << quint8(RecordTypeEnumeration::MemberReference);
            if (out.status() != QDataStream::Ok) {
                errorWriteFailed();
                return false;
            }

            // Write record
            MemberReference record;
            record.idRef = obj->idRef;

            if (!record.write(out)) {
                errorWriteFailed();
                return false;
            }
            break;
        }
        case AbstractObject::Primitive: {
            auto obj = objRef.dynamicCast<PrimitiveObject>();
            // Write record type
            out << quint8(RecordTypeEnumeration::MemberPrimitiveTyped);
            if (out.status() != QDataStream::Ok) {
                errorWriteFailed();
                return false;
            }

            // Write record
            MemberPrimitiveTyped record;
            record.value = obj->value;

            if (!record.write(out)) {
                errorWriteFailed();
                return false;
            }
            break;
        }
        case AbstractObject::String: {
            auto obj = objRef.dynamicCast<StringObject>();
            // Write record type
            out << quint8(RecordTypeEnumeration::BinaryObjectString);
            if (out.status() != QDataStream::Ok) {
                errorWriteFailed();
                return false;
            }

            // Write record
            BinaryObjectString record;
            record.objectId = obj->id;
            record.value = obj->value;

            if (!record.write(out)) {
                errorWriteFailed();
                return false;
            }
            break;
        }
        case AbstractObject::Null: {
            auto obj = objRef.dynamicCast<OneOrMoreNullObject>();
            if (obj->nullCount < 256) {
                // Write record type
                out << quint8(RecordTypeEnumeration::ObjectNullMultiple256);
                if (out.status() != QDataStream::Ok) {
                    errorWriteFailed();
                    return false;
                }

                // Write record
                ObjectNullMultiple record;
                record.nullCount = obj->nullCount;

                if (!record.write(out, true)) {
                    errorWriteFailed();
                    return false;
                }
            } else {
                // Write record type
                out << quint8(RecordTypeEnumeration::ObjectNullMultiple);
                if (out.status() != QDataStream::Ok) {
                    errorWriteFailed();
                    return false;
                }

                // Write record
                ObjectNullMultiple record;
                record.nullCount = obj->nullCount;

                if (!record.write(out, false)) {
                    errorWriteFailed();
                    return false;
                }
            }
            break;
        }
        case AbstractObject::PrimitiveList: {
            auto obj = objRef.dynamicCast<PrimitiveListObject>();
            if (obj->hasShapeInfo) {
                // Write record type
                out << quint8(RecordTypeEnumeration::BinaryArray);
                if (out.status() != QDataStream::Ok) {
                    errorWriteFailed();
                    return false;
                }

                // Write record
                if (!obj->shapeInfo.write(out)) {
                    errorWriteFailed();
                    return false;
                }
            } else {
                // Write record type
                out << quint8(RecordTypeEnumeration::ArraySinglePrimitive);
                if (out.status() != QDataStream::Ok) {
                    errorWriteFailed();
                    return false;
                }

                // Write record
                ArraySinglePrimitive record;
                record.arrayInfo.objectId = obj->id;
                record.arrayInfo.length = obj->values.size();

                if (!record.write(out)) {
                    errorWriteFailed();
                    return false;
                }
            }
            // Write contents
            if (!obj->values.write(out)) {
                errorWriteFailed();
                return false;
            }
            break;
        }
        case AbstractObject::StringList: {
            auto obj = objRef.dynamicCast<StringListObject>();
            if (obj->hasShapeInfo) {
                // Write record type
                out << quint8(RecordTypeEnumeration::BinaryArray);
                if (out.status() != QDataStream::Ok) {
                    errorWriteFailed();
                    return false;
                }

                // Write record
                if (!obj->shapeInfo.write(out)) {
                    errorWriteFailed();
                    return false;
                }
            } else {
                // Write record type
                out << quint8(RecordTypeEnumeration::ArraySingleString);
                if (out.status() != QDataStream::Ok) {
                    errorWriteFailed();
                    return false;
                }

                // Write record
                ArraySingleString record;
                record.arrayInfo.objectId = obj->id;
                record.arrayInfo.length = obj->values.size();

                if (!record.write(out)) {
                    errorWriteFailed();
                    return false;
                }
            }
            // Write contents
            for (const QString &item : qAsConst(obj->values)) {
                if (!Parser::writeString(item, out)) {
                    errorWriteFailed();
                    return false;
                }
            }
            break;
        }
        case AbstractObject::ObjectList: {
            auto obj = objRef.dynamicCast<ObjectListObject>();
            if (obj->hasShapeInfo) {
                // Write record type
                out << quint8(RecordTypeEnumeration::BinaryArray);
                if (out.status() != QDataStream::Ok) {
                    errorWriteFailed();
                    return false;
                }

                // Write record
                if (!obj->shapeInfo.write(out)) {
                    errorWriteFailed();
                    return false;
                }
            } else {
                // Write record type
                out << quint8(RecordTypeEnumeration::ArraySingleString);
                if (out.status() != QDataStream::Ok) {
                    errorWriteFailed();
                    return false;
                }

                // Write record
                ArraySingleString record;
                if (!record.write(out)) {
                    errorWriteFailed();
                    return false;
                }
            }
            // Write contents
            for (const ObjectRef &item : qAsConst(obj->values)) {
                if (!writeObject(item)) {
                    errorWriteFailed();
                    return false;
                }
            }
            break;
        }
        default:
            qDebug() << "NrbfWriter: Unsupported object type";
            break;
    }
    return true;
}

bool NrbfWriter::writeMembers(const MappingRef &mapping, const QStringList &memberNames,
                              const MemberTypeInfo &memberTypeInfo) {
    QDataStream &out = *stream;

    for (int i = 0; i < memberNames.size(); i++) {
        const QString &memberName = memberNames.at(i);
        ObjectRef objRef;

        // Find object
        {
            auto it = mapping->members.find(memberName);
            if (it == mapping->members.end()) {
                qDebug().noquote()
                    << QString("NrbfWriter: Undefined object member %1").arg(memberName);
                return false;
            }
            objRef = it.value();
        }

        if (memberTypeInfo.binaryTypeEnums[i] == BinaryTypeEnumeration::Primitive) {
            if (objRef->type() != AbstractObject::Primitive) {
                qDebug().noquote()
                    << QString("NrbfWriter: Conflicting type info and real data of member %1")
                           .arg(memberName);
                return false;
            }
            const PrimitiveValue &value = dynamic_cast<PrimitiveObject *>(objRef.data())->value;
            if (!value.write(out)) {
                return false;
            }
        } else {
            if (!writeObject(objRef)) {
                return false;
            }
        }
    }

    return true;
}

QNRBF_END_NAMESPACE