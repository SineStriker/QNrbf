#include "MemberReference.h"

#include <QDebug>

QNRBF_BEGIN_NAMESPACE

QNrbf::MemberReference::MemberReference() {
    idRef = 0;
}

bool MemberReference::read(QDataStream &in) {
    in >> idRef;
    if (idRef <= 0) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
