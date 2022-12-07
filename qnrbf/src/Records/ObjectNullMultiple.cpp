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
bool ObjectNullMultiple::write(QDataStream &out, bool is256) const {
    if (is256) {
        out << quint8(nullCount);
    } else {
        out << nullCount;
    }
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
