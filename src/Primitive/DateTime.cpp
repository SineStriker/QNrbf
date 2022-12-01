#include "DateTime.h"

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
