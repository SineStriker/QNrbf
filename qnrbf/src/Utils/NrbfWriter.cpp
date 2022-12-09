#include "NrbfWriter.h"

#include "Enums/RecordTypeEnumeration.h"
#include "Objects/DeferredReferenceObject.h"
#include "Objects/ObjectListObject.h"
#include "Objects/OneOrMoreNullObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"

#include "NrbfHelper.h"
#include "NrbfParser.h"

QNRBF_BEGIN_NAMESPACE

static void errorWriteFailed(const QString &content = QString()) {
    if (!content.isEmpty()) {
        qDebug().noquote() << QString("NrbfWriter: Write %1 failed").arg(content);
        return;
    }
    qDebug().noquote() << "NrbfWriter: Write failed";
}

static void errorWriteRecordTypeFailed() {
    errorWriteFailed("record type");
}

static bool isOneNull(const ObjectRef &obj) {
    return obj.isNull() || (obj->type() == AbstractObject::Null &&
                            dynamic_cast<OneOrMoreNullObject *>(obj.data())->nullCount == 1);
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
    if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::SerializedStreamHeader, out)) {
        errorWriteRecordTypeFailed();
        return false;
    }
    if (!reg.header->write(out)) {
        qDebug() << "NrbfWriter: Failed to write header";
        return false;
    }

    // Write libraries
    for (auto it = reg.libraries.begin(); it != reg.libraries.end(); ++it) {
        // Write record type
        if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::BinaryLibrary, out)) {
            errorWriteRecordTypeFailed();
            return false;
        }

        // Write record
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
        if (it2 == reg.objectsById.end() ||
            (it1 != reg.classesById.end() && it1.key() < it2.key())) {
            key = it1.key(), objRef = it1.value(), it1++;
        } else {
            key = it2.key(), objRef = it2.value(), it2++;
        }
        if (key < 0 || writtenObjects.contains(key)) {
            continue;
        }
        if (!writeObject(objRef)) {
            return false;
        }
    }

    // Write message end
    if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::MessageEnd, out)) {
        errorWriteRecordTypeFailed();
        return false;
    }

    return true;
}

bool NrbfWriter::writeObject(const ObjectRef &objRef) {
    QDataStream &out = *stream;
    if (isOneNull(objRef)) {
        // Write record type
        if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::ObjectNull, out)) {
            errorWriteRecordTypeFailed();
            return false;
        }
    } else {
        writtenObjects.insert(objRef->id);
        switch (objRef->type()) {
            case AbstractObject::ClassMember: {
                auto classRef = objRef.dynamicCast<ClassMemberObject>();
                switch (classRef->classType()) {
                    case ClassMemberObject::UserWithTypes: {
                        // Write record type
                        if (!Parser::writeRecordTypeEnum(
                                RecordTypeEnumeration::ClassWithMembersAndTypes, out)) {
                            errorWriteRecordTypeFailed();
                            return false;
                        }

                        // Write record
                        ClassWithMembersAndTypes record;
                        record.classInfo = classRef->classInfo;
                        record.memberTypeInfo = classRef->memberTypeInfo;
                        record.libraryId = classRef->libraryId;

                        if (!record.write(out)) {
                            qDebug().noquote()
                                << "NrbfWriter: Write ClassWithMembersAndTypes record failed.";
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
                        if (!Parser::writeRecordTypeEnum(
                                RecordTypeEnumeration::SystemClassWithMembersAndTypes, out)) {
                            errorWriteRecordTypeFailed();
                            return false;
                        }

                        // Write record
                        SystemClassWithMembersAndTypes record;
                        record.classInfo = classRef->classInfo;
                        record.memberTypeInfo = classRef->memberTypeInfo;

                        if (!record.write(out)) {
                            errorWriteFailed("SystemClassWithMembersAndTypes record");
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
                        qDebug().noquote()
                            << QString("NrbfWritter: Undefined class reference id %1")
                                   .arg(QString::number(mapping->classId));
                        return false;
                    }
                    classRef = it.value();
                }

                // Write record type
                if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::ClassWithId, out)) {
                    errorWriteRecordTypeFailed();
                    return false;
                }

                // Write record
                ClassWithId record;
                record.objectId = mapping->id;
                record.metadataId = mapping->classId;

                if (!record.write(out)) {
                    errorWriteFailed("ClassWithId record");
                    return false;
                }

                // Write members
                if (!writeMembers(mapping, classRef->classInfo.memberNames,
                                  classRef->memberTypeInfo)) {
                    return false;
                }
                break;
            }
            case AbstractObject::DeferredReference: {
                auto obj = objRef.dynamicCast<DeferredReferenceObject>();
                // Write record type
                if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::MemberReference, out)) {
                    errorWriteRecordTypeFailed();
                    return false;
                }

                // Write record
                MemberReference record;
                record.idRef = obj->idRef;

                if (!record.write(out)) {
                    errorWriteFailed("MemberReference record");
                    return false;
                }
                break;
            }
            case AbstractObject::Primitive: {
                auto obj = objRef.dynamicCast<PrimitiveObject>();
                // Write record type
                if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::MemberPrimitiveTyped,
                                                 out)) {
                    errorWriteRecordTypeFailed();
                    return false;
                }

                // Write record
                MemberPrimitiveTyped record;
                record.value = obj->value;

                if (!record.write(out)) {
                    errorWriteFailed("MemberPrimitiveTyped record");
                    return false;
                }
                break;
            }
            case AbstractObject::String: {
                auto obj = objRef.dynamicCast<StringObject>();
                // Write record type
                if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::BinaryObjectString, out)) {
                    errorWriteRecordTypeFailed();
                    return false;
                }

                // Write record
                BinaryObjectString record;
                record.objectId = obj->id;
                record.value = obj->value;

                if (!record.write(out)) {
                    errorWriteFailed("BinaryObjectString record");
                    return false;
                }
                break;
            }
            case AbstractObject::Null: {
                auto obj = objRef.dynamicCast<OneOrMoreNullObject>();
                if (!writeNullObjects(obj->nullCount)) {
                    return false;
                }
                break;
            }
            case AbstractObject::PrimitiveList: {
                auto obj = objRef.dynamicCast<PrimitiveListObject>();
                if (obj->hasShapeInfo) {
                    // Write record type
                    if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::BinaryArray, out)) {
                        errorWriteRecordTypeFailed();
                        return false;
                    }

                    // Write record
                    if (!obj->shapeInfo.write(out)) {
                        errorWriteFailed("BinaryArray record");
                        return false;
                    }
                } else {
                    // Write record type
                    if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::ArraySinglePrimitive,
                                                     out)) {
                        errorWriteRecordTypeFailed();
                        return false;
                    }

                    // Write record
                    ArraySinglePrimitive record;
                    record.arrayInfo.objectId = obj->id;
                    record.arrayInfo.length = obj->values.size();
                    record.primitiveTypeEnum = obj->values.type();

                    if (!record.write(out)) {
                        errorWriteFailed("ArraySinglePrimitive record");
                        return false;
                    }
                }
                // Write contents
                if (!obj->values.write(out)) {
                    errorWriteFailed("BinaryArray record");
                    return false;
                }
                break;
            }
            case AbstractObject::StringList: {
                auto obj = objRef.dynamicCast<StringListObject>();
                if (obj->hasShapeInfo) {
                    // Write record type
                    if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::BinaryArray, out)) {
                        errorWriteRecordTypeFailed();
                        return false;
                    }

                    // Write record
                    if (!obj->shapeInfo.write(out)) {
                        errorWriteFailed("BinaryArray record");
                        return false;
                    }
                } else {
                    // Write record type
                    if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::ArraySingleString,
                                                     out)) {
                        errorWriteRecordTypeFailed();
                        return false;
                    }

                    // Write record
                    ArraySingleString record;
                    record.arrayInfo.objectId = obj->id;
                    record.arrayInfo.length = obj->values.size();

                    if (!record.write(out)) {
                        errorWriteFailed("ArraySingleString record");
                        return false;
                    }
                }
                // Write contents
                for (const QString &item : qAsConst(obj->values)) {
                    if (!Parser::writeString(item, out)) {
                        errorWriteFailed("string");
                        return false;
                    }
                }
                break;
            }
            case AbstractObject::ObjectList: {
                auto obj = objRef.dynamicCast<ObjectListObject>();
                if (obj->hasShapeInfo) {
                    // Write record type
                    if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::BinaryArray, out)) {
                        errorWriteRecordTypeFailed();
                        return false;
                    }

                    // Write record
                    if (!obj->shapeInfo.write(out)) {
                        errorWriteFailed("BinaryArray record");
                        return false;
                    }
                } else {
                    // Write record type
                    if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::ArraySingleObject,
                                                     out)) {
                        errorWriteRecordTypeFailed();
                        return false;
                    }

                    // Write record
                    ArraySingleObject record;
                    record.arrayInfo.objectId = obj->id;
                    record.arrayInfo.length = obj->values.size();

                    if (!record.write(out)) {
                        errorWriteFailed("ArraySingleObject record");
                        return false;
                    }
                }
                {
                    // Collect null ranges
                    std::list<QPair<int, int>> nullRanges;
                    int start = 0;
                    int len = 0;
                    const auto &list = obj->values;
                    for (int i = 0; i < list.size(); ++i) {
                        if (!isOneNull(list.at(i))) {
                            if (len > 1) {
                                nullRanges.push_back(qMakePair(start, len));
                            }
                            start = i + 1;
                            len = 0;
                        } else {
                            len++;
                        }
                    }
                    if (len > 1) {
                        nullRanges.push_back(qMakePair(start, len));
                    }

                    // Write contents
                    auto it = nullRanges.begin();
                    for (int i = 0; i < list.size(); ++i) {
                        // Iterate to next continuous begin
                        while (it != nullRanges.end() && it->first < i) {
                            it++;
                        }
                        if (it != nullRanges.end() && it->first == i) {
                            // Write continuous null objects
                            if (!writeNullObjects(it->second)) {
                                return false;
                            }
                            i += len - 1;
                        } else {
                            // Write single object
                            if (!writeObject(list.at(i))) {
                                errorWriteFailed("object");
                                return false;
                            }
                        }
                    }
                }
                break;
            }
            default:
                qDebug() << "NrbfWriter: Unsupported object type";
                break;
        }
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

bool NrbfWriter::writeNullObjects(int nullCount) {
    QDataStream &out = *stream;

    if (nullCount < 256) {
        // Write record type
        if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::ObjectNullMultiple256, out)) {
            errorWriteRecordTypeFailed();
            return false;
        }

        // Write record
        ObjectNullMultiple record;
        record.nullCount = nullCount;

        if (!record.write(out, true)) {
            errorWriteFailed("ObjectNullMultiple record");
            return false;
        }
    } else {
        // Write record type
        if (!Parser::writeRecordTypeEnum(RecordTypeEnumeration::ObjectNullMultiple, out)) {
            errorWriteRecordTypeFailed();
            return false;
        }

        // Write record
        ObjectNullMultiple record;
        record.nullCount = nullCount;

        if (!record.write(out, false)) {
            errorWriteFailed("ObjectNullMultiple record");
            return false;
        }
    }

    return true;
}

QNRBF_END_NAMESPACE