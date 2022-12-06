#include "JsonReader.h"

#include <QJsonArray>
#include <QVariant>

#include "Objects/DeferredReferenceObject.h"
#include "Objects/ObjectListObject.h"
#include "Objects/OneOrMoreNullObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"
#include "Objects/SystemClassObject.h"

#include <list>

QNRBF_BEGIN_NAMESPACE

JsonReader::JsonReader(const NrbfRegistry &reg) : reg(reg) {
}

JsonReader::~JsonReader() {
}

bool JsonReader::load() {
    auto root = reg.findReference(reg.header->rootId);
    if (root.isNull()) {
        return false;
    }
    //    jsonObj = dfs(root).toObject();
    jsonObj = dfs_shallow().toObject();
    return true;
}

struct Receiver {
    enum Type {
        Mapping,
        List,
        Value,
    };
    Type type;

    // If Value
    QString key;
    QJsonValue value;

    // If Not Value
    int count;

    // If Mapping
    AbstractObject *obj;

    Receiver(const QString &key, Type type, int count, AbstractObject *obj)
        : type(type), key(key), count(count), obj(obj) {
    }
    Receiver(const QString &key, const QJsonValue &value)
        : type(Value), key(key), value(value), obj(nullptr) {
    }
};

QJsonValue JsonReader::dfs_shallow() {
    QHash<int, ObjectRef> objects = reg.objectsById;
    for (auto it = reg.classesById.begin(); it != reg.classesById.end(); ++it) {
        objects.insert(it.key(), it.value()->value);
    }

    // Objects
    QJsonObject allObj;
    for (auto it0 = objects.begin(); it0 != objects.end(); ++it0) {
        auto rootKey = it0.key();
        auto rootObj = it0.value();

        if (rootKey < 0) {
            continue;
        }

        std::list<QPair<QString, ObjectRef>> stack;
        std::list<Receiver> receivers;

        // Preorder traversal
        stack.emplace_back(QString(), rootObj);
        while (!stack.empty()) {
            auto pair = stack.back();
            stack.pop_back();

            auto objKey = pair.first;
            auto binObj = pair.second;

            if (binObj.isNull()) {
                // Null
                receivers.emplace_back(objKey, QJsonValue::Null);
            } else {
                // Extract class object
                if (binObj->type() == AbstractObject::ClassMember) {
                    binObj = dynamic_cast<ClassMemberObject *>(binObj.data())->value;
                }

                // Object
                switch (binObj->type()) {
                    case AbstractObject::DeferredReference: {
                        receivers.emplace_back(
                            objKey,
                            QJsonObject(
                                {{"idRef",
                                  dynamic_cast<DeferredReferenceObject *>(binObj.data())->id}}));
                        break;
                    }
                    case AbstractObject::Mapping: {
                        auto mapping = dynamic_cast<MappingObject *>(binObj.data());

                        // Add mapping sign
                        receivers.emplace_back(objKey, Receiver::Mapping, mapping->members.size(),
                                               mapping);

                        // Push members
                        for (auto it = mapping->members.begin(); it != mapping->members.end();
                             ++it) {
                            stack.emplace_back(it.key(), it.value());
                        }
                        break;
                    }
                    case AbstractObject::Primitive: {
                        auto val = dynamic_cast<PrimitiveObject *>(binObj.data());
                        switch (val->value.type()) {
                            case PrimitiveTypeEnumeration::Byte:
                            case PrimitiveTypeEnumeration::SByte:
                            case PrimitiveTypeEnumeration::UInt16:
                            case PrimitiveTypeEnumeration::Int16:
                            case PrimitiveTypeEnumeration::Int32:
                                receivers.emplace_back(
                                    objKey,
                                    QJsonObject(
                                        {{"type", Parser::strPrimitiveTypeEnum(val->value.type())},
                                         {"value", val->value.asString().toInt()}}));
                                break;
                            case PrimitiveTypeEnumeration::Double:
                            case PrimitiveTypeEnumeration::Single:
                                receivers.emplace_back(
                                    objKey,
                                    QJsonObject(
                                        {{"type", Parser::strPrimitiveTypeEnum(val->value.type())},
                                         {"value", val->value.asString().toDouble()}}));
                                break;
                            default:
                                receivers.emplace_back(
                                    objKey,
                                    QJsonObject(
                                        {{"type", Parser::strPrimitiveTypeEnum(val->value.type())},
                                         {"value", val->value.asString()}}));
                                break;
                        }
                        break;
                    }
                    case AbstractObject::String: {
                        auto val = dynamic_cast<StringObject *>(binObj.data());
                        receivers.emplace_back(objKey, val->value);
                        break;
                    }
                    case AbstractObject::Null: {
                        auto val = dynamic_cast<OneOrMoreNullObject *>(binObj.data());
                        if (val->nullCount == 1) {
                            receivers.emplace_back(objKey, QJsonValue::Null);
                        } else {
                            QJsonArray arr;
                            for (int i = 0; i < val->nullCount; ++i) {
                                arr.append(QJsonValue::Null);
                            }
                            receivers.emplace_back(objKey, arr);
                        }
                        break;
                    }
                    case AbstractObject::PrimitiveList: {
                        auto val = dynamic_cast<PrimitiveListObject *>(binObj.data());
                        switch (val->values.type()) {
                            case PrimitiveTypeEnumeration::Byte:
                            case PrimitiveTypeEnumeration::SByte:
                            case PrimitiveTypeEnumeration::UInt16:
                            case PrimitiveTypeEnumeration::Int16:
                            case PrimitiveTypeEnumeration::Int32: {
                                QStringList strs = val->values.asStringList();
                                QJsonArray arr;
                                for (const QString &item : qAsConst(strs)) {
                                    arr.append(item.toInt());
                                }
                                receivers.emplace_back(
                                    objKey,
                                    QJsonObject(
                                        {{"array", val->arrayType},
                                         {"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                                         {"values", arr}}));
                                break;
                            }
                            case PrimitiveTypeEnumeration::Double:
                            case PrimitiveTypeEnumeration::Single: {
                                QStringList strs = val->values.asStringList();
                                QJsonArray arr;
                                for (const QString &item : qAsConst(strs)) {
                                    arr.append(item.toDouble());
                                }
                                receivers.emplace_back(
                                    objKey,
                                    QJsonObject(
                                        {{"array", val->arrayType},
                                         {"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                                         {"values", arr}}));
                                break;
                            }
                            default:
                                receivers.emplace_back(
                                    objKey,
                                    QJsonObject(
                                        {{"array", val->arrayType},
                                         {"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                                         {"values", QJsonArray::fromStringList(
                                                        val->values.asStringList())}}));
                                break;
                        }

                        break;
                    }
                    case AbstractObject::StringList: {
                        auto val = dynamic_cast<StringListObject *>(binObj.data());
                        receivers.emplace_back(
                            objKey,
                            QJsonObject({{"array", val->arrayType},
                                         {"values", QJsonArray::fromStringList(val->values)}}));
                        break;
                    }
                    case AbstractObject::ObjectList: {
                        auto list = dynamic_cast<ObjectListObject *>(binObj.data());

                        // Add list sign
                        receivers.emplace_back(objKey, Receiver::List, list->values.size(), list);

                        // Push members reversely
                        for (auto it = list->values.rbegin(); it != list->values.rend(); ++it) {
                            static int idx = 0;

                            idx++;
                            // No key
                            stack.emplace_back("arrItem_" + QString::number(idx), *it);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }
        }

        // Rebuild tree
        QJsonValue docVal;
        {
            std::list<Receiver> anotherStack;
            while (!receivers.empty()) {
                auto top = receivers.back();
                receivers.pop_back();

                switch (top.type) {
                    case Receiver::Mapping: {
                        QJsonObject obj;
                        for (int i = 0; i < top.count; ++i) {
                            auto nextItem = anotherStack.back();
                            anotherStack.pop_back();
                            obj.insert(nextItem.key, nextItem.value);
                        }
                        auto mapping = dynamic_cast<MappingObject *>(top.obj);
                        top.value = QJsonObject({{"className", mapping->typeName},
                                                 {"assemblyName", mapping->assemblyName},
                                                 {"debugParent", mapping->parentInfo},
                                                 {"id", top.obj->id},
                                                 {"members", obj}});
                        // Receive members over
                        top.type = Receiver::Value;
                        break;
                    }
                    case Receiver::List: {
                        QJsonArray arr;
                        for (int i = 0; i < top.count; ++i) {
                            auto nextItem = anotherStack.back();
                            anotherStack.pop_back();
                            arr.append(nextItem.value);
                        }
                        auto listObj = dynamic_cast<ObjectListObject *>(top.obj);
                        top.value = QJsonObject({
                            {"array", listObj->arrayType},
                            {"values", arr},
                        });

                        // Receive members over
                        top.type = Receiver::Value;
                        break;
                    }
                    case Receiver::Value:
                        break;
                }
                anotherStack.push_back(top);
            }

            Q_ASSERT(anotherStack.size() == 1);

            docVal = anotherStack.front().value;
        }

        allObj.insert(QString("%1").arg(rootKey, (rootKey >= 0) ? 4 : 5, 10, QLatin1Char('0')),
                      docVal);
    }

    // Libraries
    QJsonObject allLibs;
    {
        for (auto it = reg.libraries.begin(); it != reg.libraries.end(); ++it) {
            allLibs.insert(QString("%1").arg(it.key(), 4, 10, QLatin1Char('0')), it.value());
        }
    }

    // Classes
    QJsonObject allClasses;
    {
        for (auto it = reg.classesById.begin(); it != reg.classesById.end(); ++it) {
            auto classRef = it.value();

            QJsonObject classObj;
            QString classType;
            switch (classRef->classType()) {
                case QNrbf::ClassMemberObject::User:
                    classType = "ClassWithMembers";
                    break;
                case QNrbf::ClassMemberObject::UserWithTypes:
                    classType = "ClassWithMembersAndTypes";
                    break;
                case QNrbf::ClassMemberObject::System:
                    classType = "SystemClassWithMembers";
                    break;
                case QNrbf::ClassMemberObject::SystemWithTypes:
                    classType = "SystemClassWithMembersAndTypes";
                    break;
                default:
                    break;
            }
            classObj.insert("recordType", classType);
            classObj.insert(
                "classInfo",
                QJsonObject({
                    {"objectId", classRef->classInfo.objectId},
                    {"name", classRef->classInfo.name},
                    {"memberCount", classRef->classInfo.memberCount},
                    {"memberNames", QJsonArray::fromStringList(classRef->classInfo.memberNames)},
                }));
            classObj.insert("libraryId", classRef->libraryId);

            allClasses.insert(QString("%1").arg(it.key(), 4, 10, QLatin1Char('0')), classObj);
        }
    }

    QJsonObject resObj;
    resObj.insert("header", QJsonObject({
                                {"rootId", reg.header->rootId},
                                {"headerId", reg.header->headerId},
                                {"majorVersion", reg.header->majorVersion},
                                {"minorVersion", reg.header->minorVersion},
                            }));
    resObj.insert("objects", allObj);
    resObj.insert("classes", allClasses);
    resObj.insert("libraries", allLibs);

    return resObj;
}

QNRBF_END_NAMESPACE