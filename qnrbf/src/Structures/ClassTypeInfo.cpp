#include "ClassTypeInfo.h"

#include "Primitive/Parser.h"

QNRBF_BEGIN_NAMESPACE

ClassTypeInfo::ClassTypeInfo() {
    libraryId = 0;
}

ClassTypeInfo::ClassTypeInfo(const QString &typeName, qint32 libraryId)
    : typeName(typeName), libraryId(libraryId) {
}

bool ClassTypeInfo::read(QDataStream &in) {
    if (!Parser::readString(typeName, in)) {
        return false;
    }
    in >> libraryId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE