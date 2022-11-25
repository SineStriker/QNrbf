#ifndef QNRBF_CLASSTYPEINFO_H
#define QNRBF_CLASSTYPEINFO_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.8 ClassTypeInfo

class QNRBF_INTERNAL ClassTypeInfo {
public:
    ClassTypeInfo();

    QString typeName;
    qint32 libraryId;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

Q_DECLARE_METATYPE(QNrbf::ClassTypeInfo);

#endif // QNRBF_CLASSTYPEINFO_H
