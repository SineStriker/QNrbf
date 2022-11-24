#ifndef QNRBF_MESSAGEFLAGS_H
#define QNRBF_MESSAGEFLAGS_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.2.1.1 MessageFlags

enum class MessageFlags : quint32 {
    NoArgs = 0x00000001,
    ArgsInline = 0x00000002,
    ArgsIsArray = 0x00000004,
    ArgsInArray = 0x00000008,
    NoContext = 0x00000010,
    ContextInline = 0x00000020,
    ContextInArray = 0x00000040,
    MethodSignatureInArray = 0x00000080,
    PropertiesInArray = 0x00000100,
    NoReturnValue = 0x00000200,
    ReturnValueVoid = 0x00000400,
    ReturnValueInline = 0x00000800,
    ReturnValueInArray = 0x00001000,
    ExceptionInArray = 0x00002000,
    GenericMethod = 0x00008000,
};

QNRBF_END_NAMESPACE

Q_DECLARE_METATYPE(QNrbf::MessageFlags);

#endif // QNRBF_MESSAGEFLAGS_H
