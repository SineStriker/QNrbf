#include "NrbfRegistry.h"

#include <QDebug>

#include "Objects/ClassMemberObject.h"
#include "Objects/DeferredReferenceObject.h"
#include "Objects/ObjectListObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"

QNRBF_BEGIN_NAMESPACE

NrbfRegistry::NrbfRegistry() {
}

void NrbfRegistry::reset() {
    objectsById.clear();
    classesById.clear();
    libraries.clear();
    header.clear();
}

ObjectRef NrbfRegistry::findReference(qint32 id) const {
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


QString NrbfRegistry::toBackingField(const QString &key) {
    return QString("<%1>k__BackingField").arg(key);
}

bool NrbfRegistry::findString(const ObjectMap &objMap, const QString &key, QString &out) const {
    const ObjectRef &objRef = findRealObject(objMap, key);
    if (objRef.isNull()) {
        // Treat as default if null
        out.clear();
    } else if (objRef->type() == AbstractObject::String) {
        out = dynamic_cast<StringObject *>(objRef.data())->value;
    } else {
        return false;
    }
    return true;
}

bool NrbfRegistry::findPrimitive(const ObjectMap &objMap, const QString &key,
                                 PrimitiveValue &out) const {
    const ObjectRef &objRef = findRealObject(objMap, key);
    if (objRef.isNull()) {
        // Treat as default if null
        out = PrimitiveValue();
    } else if (objRef->type() == AbstractObject::Primitive) {
        out = dynamic_cast<PrimitiveObject *>(objRef.data())->value;
    } else {
        return false;
    }
    return true;
}

bool NrbfRegistry::findObject(const ObjectMap &objMap, const QString &key, MappingRef &out) const {
    const ObjectRef &objRef = findRealObject(objMap, key);
    if (objRef.isNull()) {
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

bool NrbfRegistry::findStringList(const ObjectMap &objMap, const QString &key,
                                  QStringList &out) const {
    const ObjectRef &objRef = findRealObject(objMap, key);
    if (objRef.isNull()) {
        // Treat as default if null
        out.clear();
    } else if (objRef->type() == AbstractObject::StringList) {
        out = dynamic_cast<StringListObject *>(objRef.data())->values;
    } else {
        return false;
    }
    return true;
}

bool NrbfRegistry::findPrimitiveList(const ObjectMap &objMap, const QString &key,
                                     PrimitiveValueArray &out) const {
    const ObjectRef &objRef = findRealObject(objMap, key);
    if (objRef.isNull()) {
        // Treat as default if null
        out = PrimitiveValueArray();
    } else if (objRef->type() == AbstractObject::PrimitiveList) {
        out = dynamic_cast<PrimitiveListObject *>(objRef.data())->values;
    } else {
        return false;
    }
    return true;
}

bool NrbfRegistry::findObjectList(const ObjectMap &objMap, const QString &key,
                                  QList<ObjectRef> &out) const {
    const ObjectRef &objRef = findRealObject(objMap, key);
    if (objRef.isNull()) {
        // Treat as default if null
        out.clear();
    } else if (objRef->type() == AbstractObject::ObjectList) {
        out = dynamic_cast<ObjectListObject *>(objRef.data())->values;
    } else {
        return false;
    }
    return true;
}

ObjectRef NrbfRegistry::resolveReference(ObjectRef obj) const {
    if (obj.isNull() || obj->type() == AbstractObject::Null) {
        return {};
    } else if (obj->type() == AbstractObject::DeferredReference) {
        // Recursively find reference
        while (!obj.isNull() && obj->type() == AbstractObject::DeferredReference) {
            obj = findReference(dynamic_cast<DeferredReferenceObject *>(obj.data())->id);
        }
    }
    return obj;
}

ObjectRef NrbfRegistry::findRealObject(const ObjectMap &objMap, const QString &key) const {
    auto it = objMap.find(key);
    if (it == objMap.end()) {
        return {};
    }
    return resolveReference(it.value());
}

QNRBF_END_NAMESPACE