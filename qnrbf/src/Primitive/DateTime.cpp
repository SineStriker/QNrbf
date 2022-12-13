#include "Primitive/DateTime.h"

QNRBF_BEGIN_NAMESPACE

DateTime::DateTime() : DateTime(0) {
}

DateTime::DateTime(quint64 data) : _data(data) {
}

DateTime::DateTime(const DateTime &other) {
    _data = other._data;
}

DateTime::DateTime(DateTime &&other) noexcept {
    _data = std::move(other._data);
}

DateTime &DateTime::operator=(const DateTime &other) {
    _data = other._data;
    return *this;
}

DateTime &DateTime::operator=(DateTime &&other) noexcept {
    _data = std::move(other._data);
    return *this;
}

qint64 DateTime::ticks() const {
    qint64 tmp = qint64(_data) << 2;
    return tmp >> 2;
}
DateTime::Kind DateTime::kind() const {
    Kind res = static_cast<Kind>(_data >> 62);
    if (res > Local || res < Unspecified){
        res = Unspecified;
    }
    return res;
}

QNRBF_END_NAMESPACE
