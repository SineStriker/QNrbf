#include "ArrayOfValueWithCode.h"

QNRBF_BEGIN_NAMESPACE

bool ArrayOfValueWithCode::read(QDataStream &in) {
    qint32 len;
    in >> len;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    for (qint32 i = 0; i < len; ++i) {
        QNrbf::ValueWithCode val;
        if (!val.read(in)) {
            return false;
        }
        values.append(val);
    }
    return true;
}

QNRBF_END_NAMESPACE
