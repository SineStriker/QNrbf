#include "JsonReader.h"

#include <QJsonArray>

#include "Objects/DeferredReferenceObject.h"
#include "Objects/ObjectListObject.h"
#include "Objects/OneOrMoreNullObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"
#include "Objects/SystemClassObject.h"

QNRBF_BEGIN_NAMESPACE

JsonReader::JsonReader(const NrbfRegistry &reg) : reg(reg) {
}

bool JsonReader::load() {
    auto root = reg.findReference(reg.header->rootId);
    if (root.isNull()) {
        return false;
    }
    jsonObj = dfs(root).toObject();
    return true;
}

QJsonValue JsonReader::dfs(const ObjectRef &binObj) {
    QJsonValue res;
    if (binObj.isNull()) {
        return "__null__";
    }
    if (cachedObjs.contains(binObj.data())) {
        QJsonObject obj;
        obj.insert("idRef", binObj->id);
        res = obj;
    } else {
        cachedObjs.insert(binObj.data());

        switch (binObj->type()) {
            case AbstractObject::DeferredReference: {
                auto ref = dynamic_cast<DeferredReferenceObject *>(binObj.data());
                //                if (cachedIds.contains(ref->id)) {
                QJsonObject obj;
                obj.insert("idRef", ref->id);
                res = obj;
                //                } else {
                //                    cachedIds.insert(ref->id);
                //                    res = dfs(reg.resolveReference(binObj));
                //                }
                //                res = dfs(reg.resolveReference(binObj));
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
                                         {"value", arr}});
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
                                         {"value", arr}});
                        break;
                    }
                    default:
                        res = QJsonObject(
                            {{"type", Parser::strPrimitiveTypeEnum(val->values.type())},
                             {"value", QJsonArray::fromStringList(val->values.asStringList())}});
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

QNRBF_END_NAMESPACE