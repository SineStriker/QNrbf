#include "QNrbfValue.h"

#include "Utils/NrbfContainer.h"

QNrbfValue::QNrbfValue(const QNrbfValue &other) : d(new QNrbfContainerPrivate()) {
}

QNrbfValue &QNrbfValue::operator=(const QNrbfValue &other) {
    return *this;
}

QNrbfValue::~QNrbfValue() {
}

QNrbfValue::Type QNrbfValue::type() const {
    return QNrbfValue::Invalid;
}

bool QNrbfValue::isArray() const {
    return false;
}

QNrbfValue::QNrbfValue() {
}

void *QNrbfValue::data() {
    return nullptr;
}

const void *QNrbfValue::constData() const {
    return nullptr;
}
