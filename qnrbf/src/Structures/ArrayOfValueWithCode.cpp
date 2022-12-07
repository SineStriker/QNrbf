#include "ArrayOfValueWithCode.h"

QNRBF_BEGIN_NAMESPACE

bool ArrayOfValueWithCode::read(QDataStream &in) {
    qint32 len;
    in >> len;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    values.reserve(len);
    for (qint32 i = 0; i < len; ++i) {
        QNrbf::ValueWithCode val;
        if (!val.read(in)) {
            return false;
        }
        values.append(val);
    }
    return true;
}

bool ArrayOfValueWithCode::write(QDataStream &out) const {
    out << qint32(values.size());
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    for (const auto &item : qAsConst(values)) {
        if (!item.write(out)) {
            return false;
        }
    }
    return false;
}

QNRBF_END_NAMESPACE
