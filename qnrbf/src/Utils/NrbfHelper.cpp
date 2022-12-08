#include "NrbfHelper.h"

QNRBF_BEGIN_NAMESPACE

QString Helper::toBackingField(const QString &key) {
    return QString("<%1>k__BackingField").arg(key);
}

QNRBF_END_NAMESPACE
