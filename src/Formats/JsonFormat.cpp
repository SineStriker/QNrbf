#include "JsonFormat.h"

#include <QJsonArray>

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

QNRBF_BEGIN_NAMESPACE

bool JsonFormat::load(const QNrbf::ObjectRef &in) {
    jsonObj = dfs(in).toObject();
    return true;
}

bool JsonFormat::save(ObjectRef &out) {
    return false;
}

QJsonValue JsonFormat::dfs(const ObjectRef &binObj) {
    QJsonValue res;
    if (binObj.isNull()) {
        return "__null__";
    }
    switch (binObj->type()) {
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
            auto primitive = dynamic_cast<PrimitiveObject *>(binObj.data());
            switch (primitive->value.type()) {
                case PrimitiveTypeEnumeration::Byte:
                case PrimitiveTypeEnumeration::SByte:
                case PrimitiveTypeEnumeration::UInt16:
                case PrimitiveTypeEnumeration::Int16:
                case PrimitiveTypeEnumeration::Int32:
                    res = primitive->value.asString().toInt();
                    break;
                case PrimitiveTypeEnumeration::Double:
                case PrimitiveTypeEnumeration::Single:
                    res = primitive->value.asString().toDouble();
                    break;
                default:
                    res = primitive->value.asString();
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
            auto list = dynamic_cast<PrimitiveListObject *>(binObj.data());
            switch (list->values.type()) {
                case PrimitiveTypeEnumeration::Byte:
                case PrimitiveTypeEnumeration::SByte:
                case PrimitiveTypeEnumeration::UInt16:
                case PrimitiveTypeEnumeration::Int16:
                case PrimitiveTypeEnumeration::Int32: {
                    QStringList strs = list->values.asStringList();
                    QJsonArray arr;
                    for (const QString &item : qAsConst(strs)) {
                        arr.append(item.toInt());
                    }
                    res = arr;
                    break;
                }
                case PrimitiveTypeEnumeration::Double:
                case PrimitiveTypeEnumeration::Single: {
                    QStringList strs = list->values.asStringList();
                    QJsonArray arr;
                    for (const QString &item : qAsConst(strs)) {
                        arr.append(item.toDouble());
                    }
                    res = arr;
                    break;
                }
                default:
                    res = QJsonArray::fromStringList(list->values.asStringList());
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
    return res;
}

QNRBF_END_NAMESPACE