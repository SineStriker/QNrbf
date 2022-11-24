#ifndef QNRBF_LENGTHPREFIXEDSTRING_H
#define QNRBF_LENGTHPREFIXEDSTRING_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.6 LengthPrefixedString

namespace Parser {

    QNRBF_API bool readLengthPrefix(quint32 &size, QDataStream &in);

    QNRBF_API bool readString(QString &out, QDataStream &in);

}; // namespace Parser

QNRBF_END_NAMESPACE

#endif // QNRBF_LENGTHPREFIXEDSTRING_H
