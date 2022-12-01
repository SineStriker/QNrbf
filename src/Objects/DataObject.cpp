#include "DataObject.h"

QNRBF_BEGIN_NAMESPACE

DataObject::DataObject() : AbstractObject(Data) {
}

DataObject::DataObject(const PrimitiveValue &data) : AbstractObject(Data), data(data) {
}

QNRBF_END_NAMESPACE
