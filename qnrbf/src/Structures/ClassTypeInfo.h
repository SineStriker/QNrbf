#ifndef QNRBF_CLASSTYPEINFO_H
#define QNRBF_CLASSTYPEINFO_H

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.8 ClassTypeInfo

class QNRBF_INTERNAL ClassTypeInfo {
public:
    ClassTypeInfo();
    ClassTypeInfo(const QString &typeName, qint32 libraryId);

    QString typeName;
    qint32 libraryId;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSTYPEINFO_H
