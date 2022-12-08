#include "ClassInfo.h"

#include "Utils/NrbfParser.h"

QNRBF_BEGIN_NAMESPACE

ClassInfo::ClassInfo() {
    objectId = 0;
    memberCount = 0;
}

bool ClassInfo::read(QDataStream &in) {
    in >> objectId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }

    if (!Parser::readString(name, in)) {
        return false;
    }

    in >> memberCount;
    if (in.status() != QDataStream::Ok) {
        return false;
    }

    for (qint32 i = 0; i < memberCount; ++i) {
        QString str;
        if (!Parser::readString(str, in)) {
            return false;
        }
        memberNames.append(str);
    }
    return true;
}

bool ClassInfo::write(QDataStream &out) const {
    out << objectId;
    if (out.status() != QDataStream::Ok) {
        return false;
    }

    if (!Parser::writeString(name, out)) {
        return false;
    }

    out << memberCount;
    if (out.status() != QDataStream::Ok) {
        return false;
    }

    for (const auto &str : qAsConst(memberNames)) {
        if (!Parser::writeString(str, out)) {
            return false;
        }
    }
    return true;
}

QNRBF_END_NAMESPACE
