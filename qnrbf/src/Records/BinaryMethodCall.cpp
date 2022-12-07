#include "BinaryMethodCall.h"

#include "Enums/MessageFlags.h"

QNRBF_BEGIN_NAMESPACE

BinaryMethodCall::BinaryMethodCall() {
    messageEnum = 0;
}

bool BinaryMethodCall::read(QDataStream &in) {
    in >> messageEnum;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (!methodName.read(in)) {
        return false;
    }
    if (!typeName.read(in)) {
        return false;
    }
    if ((messageEnum & (quint32) QNrbf::MessageFlags::ContextInline) && !callContext.read(in)) {
        return false;
    }
    if ((messageEnum & (quint32) QNrbf::MessageFlags::ArgsInline) && !args.read(in)) {
        return false;
    }
    return true;
}

bool BinaryMethodCall::write(QDataStream &out) const {
    return false;
}

QNRBF_END_NAMESPACE
