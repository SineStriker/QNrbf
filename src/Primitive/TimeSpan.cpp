#include "TimeSpan.h"

QNRBF_BEGIN_NAMESPACE

TimeSpan::TimeSpan() {
    _data = 0;
}

bool TimeSpan::read(QDataStream &in) {
    in >> _data;
    if (in.status() != QDataStream::Ok)
        return false;
    return true;
}

QNRBF_END_NAMESPACE