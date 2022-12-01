#ifndef QNRBF_LENGTHPREFIXEDSTRING_H
#define QNRBF_LENGTHPREFIXEDSTRING_H

#include <QDataStream>

#include "Primitive/DateTime.h"
#include "Primitive/Decimal.h"
#include "Primitive/TimeSpan.h"

QNRBF_BEGIN_NAMESPACE

namespace Parser {

    QNRBF_INTERNAL bool readLengthPrefix(quint32 &size, QDataStream &in);

    QNRBF_INTERNAL bool readString(QString &out, QDataStream &in);

    QNRBF_INTERNAL bool readUtf8Char(QChar &out, QDataStream &in);

    QNRBF_INTERNAL bool readDateTime(QNrbf::DateTime &out, QDataStream &in);

    QNRBF_INTERNAL bool readDecimal(QNrbf::Decimal &out, QDataStream &in);

    QNRBF_INTERNAL bool readTimeSpan(QNrbf::TimeSpan &out, QDataStream &in);

}; // namespace Parser

QNRBF_END_NAMESPACE

#endif // QNRBF_LENGTHPREFIXEDSTRING_H
