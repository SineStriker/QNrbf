#ifndef QNRBF_PARSER_H
#define QNRBF_PARSER_H

#include <QDataStream>

#include "Primitive/DateTime.h"
#include "Primitive/Decimal.h"
#include "Primitive/TimeSpan.h"

QNRBF_BEGIN_NAMESPACE

namespace Parser {

    /* Read */
    QNRBF_INTERNAL bool readLengthPrefix(quint32 &out, QDataStream &in);

    QNRBF_INTERNAL bool readString(QString &out, QDataStream &in);

    QNRBF_INTERNAL bool readUtf8Char(QChar &out, QDataStream &in);

    QNRBF_INTERNAL bool readDateTime(QNrbf::DateTime &out, QDataStream &in);

    QNRBF_INTERNAL bool readDecimal(QNrbf::Decimal &out, QDataStream &in);

    QNRBF_INTERNAL bool readTimeSpan(QNrbf::TimeSpan &out, QDataStream &in);

    /* Write */
    QNRBF_INTERNAL bool writeLengthPrefix(quint32 size, QDataStream &out);

    QNRBF_INTERNAL bool writeString(const QString &in, QDataStream &out);

    QNRBF_INTERNAL bool writeUtf8Char(const QChar &in, QDataStream &out);

    QNRBF_INTERNAL bool writeDateTime(const QNrbf::DateTime &in, QDataStream &out);

    QNRBF_INTERNAL bool writeDecimal(const QNrbf::Decimal &in, QDataStream &out);

    QNRBF_INTERNAL bool writeTimeSpan(const QNrbf::TimeSpan &in, QDataStream &out);

}; // namespace Parser

QNRBF_END_NAMESPACE

#endif // QNRBF_PARSER_H
