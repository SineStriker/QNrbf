#include "Primitive/TimeSpan.h"

QNRBF_BEGIN_NAMESPACE

TimeSpan::TimeSpan() : TimeSpan(0) {
}

TimeSpan::TimeSpan(qint64 data) : ticks(data) {
}

TimeSpan::TimeSpan(const TimeSpan &other) {
    ticks = other.ticks;
}

TimeSpan::TimeSpan(TimeSpan &&other) noexcept {
    ticks = std::move(other.ticks);
}

TimeSpan &TimeSpan::operator=(const TimeSpan &other) {
    ticks = other.ticks;
    return *this;
}

TimeSpan &TimeSpan::operator=(TimeSpan &&other) noexcept {
    ticks = std::move(other.ticks);
    return *this;
}

QNRBF_END_NAMESPACE
