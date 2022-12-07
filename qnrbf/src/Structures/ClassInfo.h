#ifndef QNRBF_CLASSINFO_H
#define QNRBF_CLASSINFO_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.1.1 ClassInfo

class QNRBF_INTERNAL ClassInfo {
public:
    ClassInfo();

    qint32 objectId;
    QString name;
    qint32 memberCount;
    QStringList memberNames;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSINFO_H
