#include "DateTime.h"

QNRBF_BEGIN_NAMESPACE

DateTime::DateTime() {
    _data = 0;
}

int DateTime::tick() const {
    return 0;
}
int DateTime::kind() const {
    return 0;
}

bool DateTime::read(QDataStream &in) {
    in >> _data;
    if (in.status() != QDataStream::Ok)
        return false;
    return true;
}

QNRBF_END_NAMESPACE