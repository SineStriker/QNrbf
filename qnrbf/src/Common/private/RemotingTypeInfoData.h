#ifndef QNRBFFORMAT_REMOTINGTYPEINFODATA_H
#define QNRBFFORMAT_REMOTINGTYPEINFODATA_H

#include "../RemotingTypeInfo.h"
#include "Enums/PrimitiveTypeEnumeration.h"
#include "Structures/ClassTypeInfo.h"

#include <QSharedData>

QNRBF_BEGIN_NAMESPACE

class RemotingTypeInfoData : public QSharedData {
public:
    union {
        PrimitiveTypeEnumeration pte;
        ClassTypeInfo *cti;
        QString *str;
    } data;

    RemotingTypeInfo::Type type;

    RemotingTypeInfoData();
    RemotingTypeInfoData(const RemotingTypeInfoData &other);
    RemotingTypeInfoData(RemotingTypeInfoData &&other) noexcept;
    ~RemotingTypeInfoData();

    void zeroMem();
    void clone(const RemotingTypeInfoData &other);
    void swap(RemotingTypeInfoData &other);
    void clean();
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_REMOTINGTYPEINFODATA_H
