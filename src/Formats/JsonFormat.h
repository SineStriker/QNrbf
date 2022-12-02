#ifndef QNRBFFORMAT_JSONFORMAT_H
#define QNRBFFORMAT_JSONFORMAT_H

#include <QJsonObject>

#include "Objects/MappingObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL JsonFormat {
public:
    bool load(const ObjectRef &in);
    bool save(ObjectRef &out);

    QJsonObject jsonObj;

protected:
    QJsonValue dfs(const ObjectRef &binObj);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_JSONFORMAT_H
