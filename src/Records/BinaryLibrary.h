#ifndef QNRBF_BINARYLIBRARY_H
#define QNRBF_BINARYLIBRARY_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.6.2 BinaryLibrary

class QNRBF_API BinaryLibrary {
public:
    BinaryLibrary();

    qint32 libraryId;
    QString libraryName;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYLIBRARY_H
