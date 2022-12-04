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

bool JsonReader::load() {
    auto root = reg.findReference(reg.header->rootId);
    if (root.isNull()) {
        return false;
    }
    jsonObj = dfs_nonRecursive(root).toObject();
    return true;
}

QJsonValue JsonReader::dfs(const ObjectRef &binObj) {
    QJsonValue res;
    if (binObj.isNull()) {
        return QJsonValue::Null;
    }
    if (cachedObjs.contains(binObj.data())) {
        QJsonObject obj;
        obj.insert("idRef", binObj->id);
        res = obj;
    } else {
        cachedObjs.insert(binObj.data());

        switch (binObj->type()) {
            case AbstractObject::DeferredReference: {
                res = dfs(reg.resolveReference(binObj));
                break;
            }
            case AbstractObject::Mapping: {
                auto mapping = dynamic_cast<MappingObject *>(binObj.data());

                QJsonObject obj;
                obj.insert("className", mapping->typeName);
                obj.insert("assemblyName", mapping->assemblyName);

                QJsonObject membersObj;
                for (auto it = mapping->members.begin(); it != mapping->members.end(); ++it) {
                    membersObj.insert(it.key(), dfs(it.value()));
                }
                obj.insert("members", membersObj);
                res = obj;
                break;
            }
            case AbstractObject::ClassMember: {
                auto classMember = dynamic_cast<ClassMemberObject *>(binObj.data());
                res = dfs(classMember->value);
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
                        res =
                            QJsonObject({{"type", Parser::strPrimitiveTypeEnum(val->value.type())},
                                         {"value", val->value.asString().toInt()}});
                        break;
                    case PrimitiveTypeEnumeration::Double:
                    case PrimitiveTypeEnumeration::Single:
                        res =
                            QJsonObject({{"type", Parser::strPrimitiveTypeEnum(val->value.type())},
                                         {"value", val->value.asString().toDouble()}});
                        break;
                    default:
                        res =
                            QJsonObject({{"type", Parser::strPrimitiveTypeEnum(val->value.type())},
                                         {"value", val->value.asString()}});
                        break;
                }
                break;
            }
            case AbstractObject::String: {
                auto string = dynamic_cast<StringObject *>(binObj.data());
                res = string->value;
                break;
            }
            case AbstractObject::Null: {
                auto null = dynamic_cast<OneOrMoreNullObject *>(binObj.data());
                if (null->nullCount == 1) {
                    res = QJsonValue::Null;
                } else {
                    QJsonArray arr;
                    for (int i = 0; i < null->nullCount; ++i) {
                        arr.append(QJsonValue::Null);
                    }
                    res = arr;
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
                        res =
                            QJsonObject({{"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                                         {"values", arr}});
                        break;
                    }
                    case PrimitiveTypeEnumeration::Double:
                    case PrimitiveTypeEnumeration::Single: {
                        QStringList strs = val->values.asStringList();
                        QJsonArray arr;
                        for (const QString &item : qAsConst(strs)) {
                            arr.append(item.toDouble());
                        }
                        res =
                            QJsonObject({{"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                                         {"values", arr}});
                        break;
                    }
                    default:
                        res = QJsonObject(
                            {{"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                             {"values", QJsonArray::fromStringList(val->values.asStringList())}});
                        break;
                }

                break;
            }
            case AbstractObject::StringList: {
                auto list = dynamic_cast<StringListObject *>(binObj.data());
                res = QJsonArray::fromStringList(list->values);
                break;
            }
            case AbstractObject::ObjectList: {
                auto list = dynamic_cast<ObjectListObject *>(binObj.data());
                QJsonArray arr;
                for (const auto &ref : qAsConst(list->values)) {
                    arr.append(dfs(ref));
                }
                res = arr;
                break;
            }
            default:
                break;
        }
    }
    return res;
}

QJsonValue JsonReader::dfs_nonRecursive(const ObjectRef &rootObj) {
    if (rootObj.isNull()) {
        return QJsonValue::Null;
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
        QString typeName;
        QString assemblyName;

        Receiver(const QString &key, Type type, int count, const QString &typeName = QString(),
                 const QString &assemblyName = QString())
            : type(type), key(key), count(count), typeName(typeName), assemblyName(assemblyName) {
        }
        Receiver(const QString &key, const QJsonValue &value)
            : type(Value), key(key), value(value) {
        }
    };

    std::list<QPair<QString, ObjectRef>> stack;
    std::list<Receiver> receivers;
    QSet<AbstractObject *> visited;

    // Preorder traversal
    stack.emplace_back(QString(), rootObj);
    while (!stack.empty()) {
        auto pair = stack.back();
        stack.pop_back();

        auto objKey = pair.first;
        auto binObj = pair.second;

        // Resolve reference
        while (!binObj.isNull() && binObj->type() == AbstractObject::DeferredReference) {
            binObj = reg.resolveReference(binObj);
        }

        if (binObj.isNull()) {
            // Null
            receivers.emplace_back(objKey, QJsonValue::Null);
        } else if (visited.contains(binObj.data())) {
            // Repeated reference
            receivers.emplace_back(objKey, QJsonObject({{"idRef", binObj->id}}));
        } else {
            visited.insert(binObj.data());

            // Extract class object
            if (binObj->type() == AbstractObject::ClassMember) {
                binObj = dynamic_cast<ClassMemberObject *>(binObj.data())->value;
            }

            // Object
            switch (binObj->type()) {
                case AbstractObject::Mapping: {
                    auto mapping = dynamic_cast<MappingObject *>(binObj.data());

                    // Add mapping sign
                    receivers.emplace_back(objKey, Receiver::Mapping, mapping->members.size(),
                                           mapping->typeName, mapping->assemblyName);

                    // Push members
                    for (auto it = mapping->members.begin(); it != mapping->members.end(); ++it) {
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
                                    {{"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                                     {"value", arr}}));
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
                                    {{"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                                     {"value", arr}}));
                            break;
                        }
                        default:
                            receivers.emplace_back(
                                objKey,
                                QJsonObject(
                                    {{"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                                     {"value",
                                      QJsonArray::fromStringList(val->values.asStringList())}}));
                            break;
                    }

                    break;
                }
                case AbstractObject::StringList: {
                    auto val = dynamic_cast<StringListObject *>(binObj.data());
                    receivers.emplace_back(objKey, QJsonArray::fromStringList(val->values));
                    break;
                }
                case AbstractObject::ObjectList: {
                    auto list = dynamic_cast<ObjectListObject *>(binObj.data());

                    // Add list sign
                    receivers.emplace_back(objKey, Receiver::List, list->values.size());

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
                    top.value = QJsonObject({{"className", top.typeName},
                                             {"assemblyName", top.assemblyName},
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
                    top.value = arr;

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

    return docVal;
}

QNRBF_END_NAMESPACE