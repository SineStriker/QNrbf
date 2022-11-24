#include "BinaryMethodReturn.h"
#include "Enums/MessageFlags.h"

QNRBF_BEGIN_NAMESPACE

BinaryMethodReturn::BinaryMethodReturn() {
    messageEnum = 0;
}

bool BinaryMethodReturn::read(QDataStream &in) {
    in >> messageEnum;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if ((messageEnum & (quint32) QNrbf::MessageFlags::ReturnValueInline) && !returnValue.read(in)) {
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

QNRBF_END_NAMESPACE