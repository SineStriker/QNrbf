#include "NrbfContainer.h"

QNrbfContainerPrivate::QNrbfContainerPrivate() {
}

QNrbfContainerPrivate::QNrbfContainerPrivate(const QNrbfContainerPrivate &other) {
}

QNrbfContainerPrivate::QNrbfContainerPrivate(QNrbfContainerPrivate &&other) noexcept {
}

QNrbfContainerPrivate::~QNrbfContainerPrivate() {
}

void QNrbfContainerPrivate::zeroMem() {
    PrimitiveValueData::zeroMem();
}

void QNrbfContainerPrivate::clone(const QNrbfContainerPrivate &other) {
}

void QNrbfContainerPrivate::swap(QNrbfContainerPrivate &other) {
}

void QNrbfContainerPrivate::clean() {
    PrimitiveValueData::clean();
}
