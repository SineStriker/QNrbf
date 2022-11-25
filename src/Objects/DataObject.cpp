#include "DataObject.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::DataObject::DataObject(const QVariant &data) : AbstractObject(Data), data(data) {
}
QNRBF_END_NAMESPACE
