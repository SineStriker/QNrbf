#ifndef QNRBF_CLASSTYPEINFO_H
#define QNRBF_CLASSTYPEINFO_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API ClassTypeInfo {
public:
    ClassTypeInfo();

    QString typeName;
    qint32 libraryId;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

Q_DECLARE_METATYPE(QNrbf::ClassTypeInfo);

#endif // QNRBF_CLASSTYPEINFO_H
