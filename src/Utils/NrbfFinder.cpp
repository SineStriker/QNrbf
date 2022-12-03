#include "NrbfFinder.h"

#include "Objects/ClassMemberObject.h"
#include "Objects/ObjectListObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"

QNRBF_BEGIN_NAMESPACE

QString NrbfFinder::toBackingField(const QString &key) {
    return QString("<%1>k__BackingField").arg(key);
}

bool NrbfFinder::findString(const ObjectMap &objMap, const QString &key, QString &out) {
    auto it = objMap.find(key);
    if (it == objMap.end()) {
        // Treat as default if not found
        out.clear();
        return true;
    }
    const ObjectRef &objRef = it.value();
    if (objRef.isNull() || objRef->type() == AbstractObject::Null) {
        // Treat as default if null
        out.clear();
    } else if (objRef->type() == AbstractObject::String) {
        out = dynamic_cast<StringObject *>(objRef.data())->value;
    } else {
        return false;
    }
    return true;
}

bool NrbfFinder::findPrimitive(const NrbfFinder::ObjectMap &objMap, const QString &key,
                               PrimitiveValue &out) {
    auto it = objMap.find(key);
    if (it == objMap.end()) {
        // Treat as default if not found
        out = PrimitiveValue();
        return true;
    }
    const ObjectRef &objRef = it.value();
    if (objRef.isNull() || objRef->type() == AbstractObject::Null) {
        // Treat as default if null
        out = PrimitiveValue();
    } else if (objRef->type() == AbstractObject::Primitive) {
        out = dynamic_cast<PrimitiveObject *>(objRef.data())->value;
    } else {
        return false;
    }
    return true;
}

bool NrbfFinder::findObject(const NrbfFinder::ObjectMap &objMap, const QString &key,
                            MappingRef &out) {
    auto it = objMap.find(key);
    if (it == objMap.end()) {
        // Treat as default if not found
        out.reset();
        return true;
    }
    const ObjectRef &objRef = it.value();
    if (objRef.isNull() || objRef->type() == AbstractObject::Null) {
        out.reset();
    } else if (objRef->type() == AbstractObject::ClassMember) {
        out = dynamic_cast<ClassMemberObject *>(objRef.data())->value;
    } else if (objRef->type() == AbstractObject::Mapping) {
        out = objRef.dynamicCast<MappingObject>();
    } else {
        return false;
    }
    return true;
}

bool NrbfFinder::findStringList(const NrbfFinder::ObjectMap &objMap, const QString &key,
                                QStringList &out) {
    auto it = objMap.find(key);
    if (it == objMap.end()) {
        // Treat as default if not found
        out.clear();
        return true;
    }
    const ObjectRef &objRef = it.value();
    if (objRef.isNull() || objRef->type() == AbstractObject::Null) {
        // Treat as default if null
        out.clear();
    } else if (objRef->type() == AbstractObject::StringList) {
        out = dynamic_cast<StringListObject *>(objRef.data())->values;
    } else {
        return false;
    }
    return true;
}

bool NrbfFinder::findPrimitiveList(const NrbfFinder::ObjectMap &objMap, const QString &key,
                                   PrimitiveValueArray &out) {
    auto it = objMap.find(key);
    if (it == objMap.end()) {
        // Treat as default if not found
        out = PrimitiveValueArray();
        return true;
    }
    const ObjectRef &objRef = it.value();
    if (objRef.isNull() || objRef->type() == AbstractObject::Null) {
        // Treat as default if null
        out = PrimitiveValueArray();
    } else if (objRef->type() == AbstractObject::PrimitiveList) {
        out = dynamic_cast<PrimitiveListObject *>(objRef.data())->values;
    } else {
        return false;
    }
    return true;
}

bool NrbfFinder::findObjectList(const NrbfFinder::ObjectMap &objMap, const QString &key,
                                QList<ObjectRef> &out) {
    auto it = objMap.find(key);
    if (it == objMap.end()) {
        // Treat as default if not found
        out.clear();
        return true;
    }
    const ObjectRef &objRef = it.value();
    if (objRef.isNull() || objRef->type() == AbstractObject::Null) {
        // Treat as default if null
        out.clear();
    } else if (objRef->type() == AbstractObject::ObjectList) {
        out = dynamic_cast<ObjectListObject *>(objRef.data())->values;
    } else {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
