#include "ObjectNullMultiple.h"

QNRBF_BEGIN_NAMESPACE

ObjectNullMultiple::ObjectNullMultiple() {
    nullCount = 0;
}

bool ObjectNullMultiple::read(QDataStream &in, bool is256) {
    if (is256) {
        quint8 ch;
        in >> ch;

        nullCount = ch;
    } else {
        in >> nullCount;
    }
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE