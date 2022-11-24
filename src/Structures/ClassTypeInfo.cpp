#include "ClassTypeInfo.h"

#include "Primitive/LengthPrefixedString.h"

QNRBF_BEGIN_NAMESPACE

ClassTypeInfo::ClassTypeInfo() {
    libraryId = 0;
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