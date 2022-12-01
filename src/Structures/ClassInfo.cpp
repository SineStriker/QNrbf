#include "ClassInfo.h"

#include "Primitive/LengthPrefixedString.h"

QNRBF_BEGIN_NAMESPACE

ClassInfo::ClassInfo() {
    objectId = 0;
    memberCount = 0;
}

bool ClassInfo::read(QDataStream &in) {
    in >> objectId;
    if (!Parser::readString(name, in)) {
        return false;
    }
    in >> memberCount;
    for (qint32 i = 0; i < memberCount; ++i) {
        QString str;
        if (!Parser::readString(str, in)) {
            return false;
        }
        memberNames.append(str);
    }
    return true;
}

QNRBF_END_NAMESPACE
