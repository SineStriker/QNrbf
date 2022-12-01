#ifndef QNRBF_LENGTHPREFIXEDSTRING_H
#define QNRBF_LENGTHPREFIXEDSTRING_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.6 LengthPrefixedString

namespace Parser {

    QNRBF_INTERNAL bool readLengthPrefix(quint32 &size, QDataStream &in);

    QNRBF_INTERNAL bool readString(QString &out, QDataStream &in);

    QNRBF_INTERNAL bool readUtf8Char(QChar &out, QDataStream &in);

}; // namespace Parser

QNRBF_END_NAMESPACE

#endif // QNRBF_LENGTHPREFIXEDSTRING_H
